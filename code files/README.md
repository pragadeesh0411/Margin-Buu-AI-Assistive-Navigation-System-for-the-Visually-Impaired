# MarginBuu — Shoulder-Mounted AI Assistive Companion

MarginBuu is a prototype assistive system intended to help a visually impaired user detect nearby obstacles and receive concise audio and vibration guidance. It uses a phone for higher-level AI tasks and a microcontroller for time-critical distance sensing.

> **Safety statement:** This is an educational prototype, not a certified mobility aid. It must not replace a white cane, guide dog, trained mobility techniques, or a human guide. Test only in a controlled environment with a sighted assistant. Do not rely on it near traffic, stairs, drop-offs, water, or other hazards.

## Package contents

- `firmware/sensor_controller/` — Arduino firmware for an ESP32 DevKit-class board with three ultrasonic sensors, a servo scanner, and a vibration motor.
- `firmware/esp32_cam/` — ESP32-CAM (AI Thinker) camera web server for JPEG snapshots on the local Wi-Fi network.
- `android/` — starter Android app (Kotlin) that polls the sensor controller, speaks guidance using Android TextToSpeech, and can request a camera snapshot.
- `docs/` — architecture, wiring, setup, testing, troubleshooting, and roadmap.
- `config/` — example configuration and safety checklist.

## Important hardware architecture choice

An AI Thinker ESP32-CAM does not expose enough convenient GPIO pins to safely connect three HC-SR04 sensors, a servo, and a vibration driver while also using the camera. This package therefore separates the jobs:

1. **ESP32 DevKit**: reads three ultrasonic sensors, drives the servo and vibration driver, and exposes a small HTTP status endpoint.
2. **ESP32-CAM AI Thinker**: captures camera images and exposes `/capture` on the same Wi-Fi network.
3. **Android phone**: polls the sensor endpoint, speaks obstacle guidance, and can request a JPEG snapshot for future on-device image recognition.

This split is deliberate and avoids pin conflicts. If you have only one ESP32-CAM, use an I/O expander or redesign the hardware before wiring everything together.

## Quick start

1. Read `docs/SAFETY_AND_LIMITATIONS.md` and `docs/WIRING.md`.
2. Install Arduino IDE and the ESP32 board support package.
3. Edit the Wi-Fi credentials and IP settings in both firmware sketches.
4. Flash `firmware/sensor_controller/sensor_controller.ino` to an ESP32 DevKit.
5. Flash `firmware/esp32_cam/esp32_cam.ino` to an AI Thinker ESP32-CAM using a USB-to-TTL adapter.
6. Open the serial monitor at 115200 baud. Confirm distances and IP addresses.
7. Open `http://SENSOR_CONTROLLER_IP/status` and `http://ESP32_CAM_IP/capture` from a phone on the same Wi-Fi.
8. Open `android/` in Android Studio, set the sensor and camera IPs in `MainActivity.kt`, and run on a physical Android phone.
9. Perform the staged tests in `docs/TEST_PLAN.md` with a sighted assistant.

## Current implementation vs. future work

Implemented in this package:
- Left/front/right distance sensing with filtering and threshold-based guidance.
- Servo scan sweep for a forward-facing sensor mounting arrangement (optional; see wiring).
- Vibration motor control through a transistor/MOSFET driver.
- Sensor-controller HTTP JSON endpoint.
- ESP32-CAM JPEG snapshot endpoint.
- Android TextToSpeech guidance and camera snapshot request.

Not implemented as a finished product:
- Guaranteed object classification, OCR, currency recognition, color/shape identification, translation, or conversational AI.
- Automatic stair/drop-off detection.
- Certified wearable enclosure, weatherproofing, or medical/safety certification.
- Production-grade authentication, encryption, or secure remote access.

These capabilities require additional on-device ML integration, careful testing, and a privacy/safety review. No paid Gemini API is required by this starter package.
