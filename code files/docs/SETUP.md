# Setup Instructions

## Arduino IDE

1. Install Arduino IDE 2.x.
2. In Preferences, add the Espressif ESP32 boards package URL from the official Espressif Arduino documentation, then install the ESP32 board package through Boards Manager.
3. Select an appropriate ESP32 Dev Module for the sensor controller.
4. Open `firmware/sensor_controller/sensor_controller.ino`.
5. Set `WIFI_SSID` and `WIFI_PASSWORD`.
6. Upload and open Serial Monitor at 115200 baud.
7. Record the printed IP address and test `/status`.

For ESP32-CAM:
1. Open `firmware/esp32_cam/esp32_cam.ino`.
2. Set Wi-Fi credentials and select the AI Thinker ESP32-CAM board.
3. Enter flash mode using GPIO0-to-GND, upload, then remove the jumper and reset.
4. Record the printed IP and open `/capture` in a browser on the same Wi-Fi network.

## Android Studio

1. Install a recent Android Studio version.
2. Open the `android` folder as a project.
3. Let Gradle sync. The Android Gradle Plugin and Kotlin versions may need updating to match your installed Android Studio.
4. In `android/app/src/main/java/com/example/marginbuu/MainActivity.kt`, set `SENSOR_BASE_URL` and `CAMERA_CAPTURE_URL` to the IPs printed by the boards.
5. Connect an Android phone to the same Wi-Fi and enable USB debugging, or use a compatible emulator with network access to the devices.
6. Run the app and grant any requested permissions. The starter uses TextToSpeech and network access; it does not request microphone permission because wake-word listening is not implemented.

## Network notes

- Phone and boards must be on the same LAN. Guest Wi-Fi or client isolation may block device-to-device requests.
- Use DHCP reservations or configure router reservations if device IPs change.
- Do not expose the unauthenticated HTTP endpoints directly to the public internet.
