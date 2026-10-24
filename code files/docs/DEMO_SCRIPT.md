# 2–3 Minute Demo Script

1. **Introduction (15 s):** “MarginBuu is an experimental assistive companion designed to supplement obstacle awareness using distance sensors, vibration, and voice guidance.”
2. **Architecture (20 s):** Show the ESP32 DevKit sensor controller, ESP32-CAM, and Android phone. Explain that the phone is reserved for higher-level AI processing.
3. **Sensor test (40 s):** With a sighted assistant, place a broad object in front of each sensor. Show the JSON `/status` response and the app's guidance. Do not walk toward obstacles while blindfolded.
4. **Haptic output (20 s):** Demonstrate the vibration motor at a safe distance and explain the configured warning and critical thresholds.
5. **Camera test (20 s):** Open `/capture` and show that a JPEG snapshot is served. State clearly that object recognition/OCR is a future integration, not yet implemented.
6. **Limitations (20 s):** Explain ultrasonic blind spots, lack of reliable drop-off detection, and the need for a conventional mobility aid.
7. **Conclusion (10 s):** Summarize current features and planned on-device vision work.

Use actual hardware and screenshots from your own tests. Do not present fabricated results or claim unimplemented features.
