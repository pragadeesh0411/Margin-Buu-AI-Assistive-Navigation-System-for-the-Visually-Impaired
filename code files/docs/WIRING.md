# Wiring Guide

## A. ESP32 DevKit sensor controller

Use a standard ESP32 DevKit board for the sensors and actuators. Pin assignments can be changed in the sketch if your board differs.

| Module | Signal | ESP32 DevKit GPIO | Notes |
|---|---|---:|---|
| Left HC-SR04 | TRIG | 16 | Trigger output |
| Left HC-SR04 | ECHO | 17 | **Use a voltage divider**; HC-SR04 echo is typically 5 V |
| Front HC-SR04 | TRIG | 18 | Trigger output |
| Front HC-SR04 | ECHO | 19 | **Use a voltage divider** |
| Right HC-SR04 | TRIG | 21 | Trigger output |
| Right HC-SR04 | ECHO | 22 | **Use a voltage divider** |
| Servo | Signal | 23 | Power servo from a suitable external 5 V supply |
| Vibration driver | Gate/base control | 25 | Use a transistor/MOSFET driver, not direct GPIO motor power |
| All modules | GND | GND | Common ground required |

### Voltage and power warnings
- ESP32 GPIO is **3.3 V only**. Do not connect a 5 V HC-SR04 Echo output directly to GPIO. Use a resistor divider or level shifter on every Echo line.
- Servo and vibration motor must not be powered from an ESP32 GPIO. Use a suitable external supply and a driver. Join the external supply ground to ESP32 ground.
- Add a flyback diode across a brushed DC motor if your driver/module does not already include one.
- Check the exact sensor/servo module voltage requirements before powering anything.

## B. ESP32-CAM AI Thinker

Use a USB-to-TTL adapter with selectable 3.3 V logic. For flashing:
- Adapter TX -> ESP32-CAM U0R / GPIO3
- Adapter RX -> ESP32-CAM U0T / GPIO1
- Adapter GND -> GND
- Stable 5 V supply -> 5V pin (verify your specific board)
- Connect GPIO0 to GND only while entering flash mode; remove that jumper and reset to run normally.

Do not assume the adapter's 3.3 V output can supply the camera board's peak current. A stable regulated supply is recommended. Confirm your adapter logic levels and board pin labels.

## C. Servo scan option

The supplied sensor firmware includes a servo sweep helper, but the default design assumes the three sensors are fixed left/front/right. Do not mount all three sensors on a moving servo unless you deliberately redesign the mechanics and sensor scheduling. If the servo is used to scan a single sensor, adjust the wiring and firmware accordingly.
