# Test Plan and Acceptance Criteria

Perform tests with a sighted assistant in a clear indoor area. Keep a cane or other established mobility aid available. Do not test near stairs, roads, ledges, or moving machinery.

| Test | Procedure | Expected result |
|---|---|---|
| Boot | Power the sensor controller and camera | Serial monitor prints Wi-Fi IP or a clear connection failure |
| No obstacle | Keep sensors facing an open area | Status endpoint returns JSON; readings may show out-of-range as `-1` |
| Front obstacle | Place a broad flat object about 1 m in front | Front distance decreases; guidance becomes CAUTION within configured threshold |
| Critical proximity | Slowly move object closer under assistant supervision | Guidance becomes STOP; vibration duty cycle increases |
| Left/right obstacle | Repeat on each side | Corresponding side is identified in status JSON |
| Invalid echo | Point a sensor away from nearby surfaces | Invalid reading is handled without freezing the controller |
| HTTP status | Open `/status` from phone browser | Valid JSON response |
| Camera snapshot | Open `/capture` from phone browser | Browser downloads/displays a JPEG image |
| Android speech | Run app and approach a safe test object | Short voice guidance is spoken, with repetition throttled |
| Network loss | Turn off Wi-Fi temporarily | App shows connection error; it does not claim the area is clear |
| Power test | Observe rebooting when servo/motor starts | No brownouts; if resets occur, improve power supply and grounding |

## Suggested measured results table

Record real observations instead of inventing results.

| Run | Object / surface | Actual distance | Reported distance | Spoken guidance | Pass/Fail |
|---|---|---:|---:|---|---|
| 1 | Cardboard box | ____ cm | ____ cm | ____ | ____ |
| 2 | Wall | ____ cm | ____ cm | ____ | ____ |
| 3 | Fabric / soft object | ____ cm | ____ cm | ____ | ____ |
| 4 | Angled object | ____ cm | ____ cm | ____ | ____ |

Ultrasonic readings vary with target shape, angle, material, temperature, sensor placement, and cross-talk. Run several trials per condition and report limitations.
