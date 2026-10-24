# System Architecture

```text
  Left ultrasonic sensor ─┐
  Front ultrasonic sensor ├──> ESP32 DevKit ── Wi-Fi / HTTP JSON ──> Android app
  Right ultrasonic sensor ┘         │                                  │
                                    ├──> vibration driver              ├── Text-to-Speech
                                    └──> servo (optional scan)         └── snapshot request
                                                                         │
  ESP32-CAM AI Thinker ─────── Wi-Fi / HTTP JPEG snapshot <──────────────┘
          │
          └── camera image for future on-device image recognition / OCR
```

## Responsibilities

- **ESP32 DevKit:** low-latency distance measurement, simple filtering, hazard level calculation, vibration output, and status reporting.
- **ESP32-CAM:** image capture only. Its camera pins are reserved by the camera driver.
- **Android phone:** speech, UI, image display/capture requests, and future on-device inference.
- **Power system:** separate regulated rails as needed; grounds must be common when signals connect. Never power a servo from the ESP32 3.3 V pin.

## Obstacle logic

The firmware reports the nearest valid reading and assigns a guidance message:
- `STOP` when an obstacle is at or below the critical threshold.
- `CAUTION` when it is within the warning threshold.
- `CLEAR` when no measured obstacle is within the warning range.

The Android app throttles repeated spoken messages. Ultrasonic sensors can miss soft, angled, narrow, or absorbent objects and do not reliably detect holes or downward steps.
