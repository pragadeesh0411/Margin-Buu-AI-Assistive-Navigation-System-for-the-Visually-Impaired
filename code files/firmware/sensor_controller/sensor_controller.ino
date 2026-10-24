#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// ---------- USER CONFIGURATION ----------
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

constexpr int TRIG_LEFT = 16;
constexpr int ECHO_LEFT = 17;   // 5 V Echo must pass through a divider/level shifter
constexpr int TRIG_FRONT = 18;
constexpr int ECHO_FRONT = 19;  // 5 V Echo must pass through a divider/level shifter
constexpr int TRIG_RIGHT = 21;
constexpr int ECHO_RIGHT = 22;  // 5 V Echo must pass through a divider/level shifter
constexpr int SERVO_PIN = 23;
constexpr int VIBRATION_DRIVER_PIN = 25; // transistor/MOSFET driver input, not motor directly

constexpr float WARNING_DISTANCE_CM = 120.0f;
constexpr float CRITICAL_DISTANCE_CM = 55.0f;
constexpr unsigned long ECHO_TIMEOUT_US = 25000UL;
constexpr unsigned long SAMPLE_INTERVAL_MS = 100;
// ----------------------------------------

WebServer server(80);
Servo scanServo;

struct Sensor {
  const char* name;
  int trigPin;
  int echoPin;
  float distanceCm;
};

Sensor sensors[] = {
  {"left", TRIG_LEFT, ECHO_LEFT, -1.0f},
  {"front", TRIG_FRONT, ECHO_FRONT, -1.0f},
  {"right", TRIG_RIGHT, ECHO_RIGHT, -1.0f}
};

unsigned long lastSampleMs = 0;
unsigned long lastVibrationMs = 0;
bool servoAttached = false;

float readDistanceCm(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // pulseIn timeout avoids blocking forever. -1 means no valid echo.
  unsigned long duration = pulseIn(echoPin, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) return -1.0f;

  float cm = (duration * 0.0343f) / 2.0f;
  if (cm < 2.0f || cm > 400.0f) return -1.0f;
  return cm;
}

float filteredRead(Sensor &s) {
  // Small median-of-three filter. Invalid readings are excluded.
  float values[3];
  int count = 0;
  for (int i = 0; i < 3; ++i) {
    float d = readDistanceCm(s.trigPin, s.echoPin);
    if (d > 0) values[count++] = d;
    delay(25); // spacing reduces cross-talk between sensors
  }
  if (count == 0) return -1.0f;
  for (int i = 0; i < count; ++i) {
    for (int j = i + 1; j < count; ++j) {
      if (values[j] < values[i]) {
        float t = values[i]; values[i] = values[j]; values[j] = t;
      }
    }
  }
  return values[count / 2];
}

const char* guidanceFor(float d) {
  if (d < 0) return "UNKNOWN"; // never treat failed sensor as CLEAR
  if (d <= CRITICAL_DISTANCE_CM) return "STOP";
  if (d <= WARNING_DISTANCE_CM) return "CAUTION";
  return "CLEAR";
}

int vibrationDutyForNearest(float nearest) {
  if (nearest < 0 || nearest > WARNING_DISTANCE_CM) return 0;
  if (nearest <= CRITICAL_DISTANCE_CM) return 220;
  // Increasing duty cycle as distance decreases.
  float ratio = (WARNING_DISTANCE_CM - nearest) /
                (WARNING_DISTANCE_CM - CRITICAL_DISTANCE_CM);
  return constrain((int)(70 + ratio * 120), 70, 190);
}

void handleStatus() {
  String json = "{";
  json += "\"device\":\"MarginBuu sensor controller\",";
  json += "\"left_cm\":" + String(sensors[0].distanceCm, 1) + ",";
  json += "\"front_cm\":" + String(sensors[1].distanceCm, 1) + ",";
  json += "\"right_cm\":" + String(sensors[2].distanceCm, 1) + ",";
  float nearest = -1.0f;
  int nearestIndex = -1;
  for (int i = 0; i < 3; ++i) {
    float d = sensors[i].distanceCm;
    if (d > 0 && (nearest < 0 || d < nearest)) {
      nearest = d;
      nearestIndex = i;
    }
  }
  json += "\"nearest_cm\":" + String(nearest, 1) + ",";
  json += "\"nearest_direction\":\"" + String(nearestIndex >= 0 ? sensors[nearestIndex].name : "unknown") + "\",";
  json += "\"guidance\":\"" + String(guidanceFor(nearest)) + "\",";
  json += "\"warning_cm\":" + String(WARNING_DISTANCE_CM, 1) + ",";
  json += "\"critical_cm\":" + String(CRITICAL_DISTANCE_CM, 1);
  json += "}";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", json);
}

void handleRoot() {
  server.send(200, "text/plain",
    "MarginBuu sensor controller is running. Open /status for JSON. Local LAN only.");
}

void setup() {
  Serial.begin(115200);
  for (Sensor &s : sensors) {
    pinMode(s.trigPin, OUTPUT);
    pinMode(s.echoPin, INPUT);
    digitalWrite(s.trigPin, LOW);
  }

  pinMode(VIBRATION_DRIVER_PIN, OUTPUT);
  digitalWrite(VIBRATION_DRIVER_PIN, LOW);

  // Servo is optional. It is parked at center; fixed left/front/right sensors
  // do not require a scanning servo. Remove this block if servo is not installed.
  scanServo.setPeriodHertz(50);
  scanServo.attach(SERVO_PIN, 500, 2400);
  scanServo.write(90);
  servoAttached = true;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Sensor controller IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wi-Fi connection failed. Check credentials/network and reset.");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, handleStatus);
  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  server.handleClient();

  if (millis() - lastSampleMs >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = millis();

    // Sequential sensor measurements help reduce ultrasonic cross-talk.
    for (Sensor &s : sensors) {
      s.distanceCm = filteredRead(s);
    }

    float nearest = -1.0f;
    for (Sensor &s : sensors) {
      if (s.distanceCm > 0 && (nearest < 0 || s.distanceCm < nearest)) {
        nearest = s.distanceCm;
      }
    }

    int duty = vibrationDutyForNearest(nearest);
    // ESP32 Arduino core ledc API differs by version. Use digital on/off
    // fallback here for compatibility; change to LEDC PWM if intensity levels
    // are required and your core version supports the chosen LEDC API.
    if (duty == 0) {
      digitalWrite(VIBRATION_DRIVER_PIN, LOW);
    } else if (duty >= 200) {
      digitalWrite(VIBRATION_DRIVER_PIN, HIGH);
    } else {
      // Basic pulsed vibration: approximate intensity using time duty cycle.
      unsigned long phase = millis() % 500;
      digitalWrite(VIBRATION_DRIVER_PIN, phase < (unsigned long)(duty * 2) ? HIGH : LOW);
    }

    Serial.printf("L=%.1f cm F=%.1f cm R=%.1f cm guidance=%s\n",
      sensors[0].distanceCm, sensors[1].distanceCm, sensors[2].distanceCm,
      guidanceFor(nearest));
  }
}
