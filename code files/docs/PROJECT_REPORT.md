# MarginBuu — Project Report

## Abstract
MarginBuu is a prototype shoulder-mounted assistive companion intended to provide supplementary obstacle awareness for visually impaired users. Three ultrasonic sensors measure nearby objects in left, front, and right directions. A microcontroller converts measurements into simple distance-based guidance and vibration cues. An ESP32-CAM supplies snapshots, while an Android phone provides speech output and is intended to host higher-level on-device AI processing.

## Problem statement
Independent mobility can be affected by obstacles that appear within the user's immediate path. The project explores a compact electronic aid that supplements existing mobility techniques with distance sensing, audio prompts, and haptic feedback.

## Objectives
- Detect nearby obstacles in left, front, and right directions.
- Provide distance-based STOP, CAUTION, CLEAR, or UNKNOWN states.
- Use vibration as a supplementary proximity cue.
- Capture camera images for future on-device recognition and OCR.
- Keep more compute-intensive processing on the smartphone rather than the ESP32-CAM.
- Build and test a low-cost educational prototype.

## Proposed methodology
1. Ultrasonic sensors emit pulses and measure echo time.
2. Firmware converts echo time into approximate distance and rejects invalid readings.
3. A threshold-based rule assigns a guidance state.
4. The controller activates the vibration driver and publishes JSON status over the local Wi-Fi network.
5. The Android app polls the endpoint and speaks concise messages using Android TextToSpeech.
6. The ESP32-CAM serves JPEG snapshots to the phone for a later on-device vision pipeline.

## Hardware
- ESP32 DevKit-class controller
- AI Thinker ESP32-CAM module
- Three HC-SR04-compatible ultrasonic sensors
- Small servo motor (optional, depending on final mechanical arrangement)
- Vibration motor with transistor/MOSFET driver
- USB-to-TTL adapter for camera flashing
- Regulated supplies, wiring, level shifters/resistor dividers, and enclosure

## Software
- Arduino IDE / ESP32 Arduino core
- Embedded C++ firmware
- Android Studio and Kotlin
- Android TextToSpeech
- Local HTTP/JSON communication and JPEG snapshot endpoint

## Expected output
The prototype should report sensor distances, identify the nearest measured direction, provide basic spoken guidance, and vary the vibration pattern according to proximity. Actual performance must be measured using the test plan; no test results are asserted in this report.

## Limitations
Ultrasonic sensors are sensitive to surface angle and material and cannot reliably detect drop-offs. The starter Android app does not yet implement object detection, OCR, money recognition, color/shape recognition, translation, or a conversational assistant. The prototype is not certified and must not be treated as a replacement for a cane or guide dog.

## Future scope
- Add on-device object detection and OCR.
- Improve sensor filtering, fault-state handling, and user customization.
- Add battery monitoring and a secure pairing mechanism.
- Conduct supervised usability tests with accessibility specialists and intended users.
- Measure latency, false-positive/false-negative rates, battery life, and mechanical comfort.
