# Development Roadmap

## Phase 1 — reliable prototype
- Validate each sensor against measured distances.
- Add robust median filtering and sensor cross-talk scheduling.
- Verify vibration patterns are distinguishable without being uncomfortable.
- Design a secure, lightweight shoulder mount and strain relief.

## Phase 2 — on-device vision
- Add an Android image pipeline using an on-device model, such as a compatible TensorFlow Lite object detector or ML Kit feature where suitable.
- Add OCR only after image capture and text output are tested.
- Speak short, confidence-aware descriptions and provide a repeat/stop control.
- Avoid announcing uncertain labels as facts.

## Phase 3 — accessibility and usability
- Test with intended users under an approved, supervised protocol.
- Offer adjustable speech rate, volume, vibration threshold, language, and quiet mode.
- Add battery status, watchdog handling, clear connection-state warnings, and offline behavior.
- Conduct privacy review and collect consent before storing images or usage data.

## Phase 4 — reliability and validation
- Evaluate false positives/false negatives across materials, lighting, weather, and user motion.
- Test latency, battery life, thermal behavior, and mechanical stability.
- Seek specialist accessibility and safety feedback before any real-world use.
