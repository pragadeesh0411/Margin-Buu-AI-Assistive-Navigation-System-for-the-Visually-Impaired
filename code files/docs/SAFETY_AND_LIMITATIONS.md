# Safety, Privacy, and Limitations

1. MarginBuu is an experimental student prototype, not a certified mobility aid. It cannot guarantee safe navigation.
2. Never use it as the only means of detecting hazards. Ultrasonic sensors can miss narrow, soft, angled, low, overhead, transparent, or absorbent objects.
3. The current sensors do not detect downward steps, holes, curbs, or drop-offs reliably. Do not claim stair detection unless a separate sensor design is implemented and validated.
4. Audio and vibration are supplementary cues. Keep environmental hearing available and provide a way to stop the system quickly.
5. Secure all wiring and enclose sharp edges. A shoulder-mounted unit must not snag, overheat, obstruct hearing, or create a choking/entanglement risk.
6. Test with a sighted assistant, in a controlled area, and with a conventional mobility aid. Do not test in traffic or on stairs.
7. Camera images may include bystanders. Capture only when needed, avoid recording by default, obtain consent where appropriate, and do not upload images without clear user awareness.
8. HTTP endpoints in this prototype are unauthenticated and intended for a trusted local network only. Do not port-forward them or expose them to the internet.
9. Do not power motors directly from GPIO pins. Use appropriate drivers and supplies.
10. Stop testing immediately if the device resets, becomes hot, gives misleading guidance, or causes discomfort.
