# Homework 2 - Task 1: Two-LED PWM Fading (ESP32)

Two LEDs fade in and out together using the ESP32's hardware PWM (LEDC). Each LED runs at a **different PWM frequency** (1 kHz and 5 kHz), so the two frequencies can be compared. The circuit was built and tested in Wokwi, with PlatformIO and the Arduino framework.

- **Author:** HADIA IRFAN
- **Board:** ESP32 DevKit C (Wokwi: `board-esp32-devkit-c-v4`, PlatformIO: `esp32dev`)
- **Wokwi project link:** _add your link here_

## Project files

| File | Purpose |
|------|---------|
| `src/main.cpp` | Source code |
| `diagram.json` | Wokwi circuit configuration |
| `wokwi.toml` | Wokwi firmware paths |
| `platformio.ini` | PlatformIO build settings |

## Components

- 1 x ESP32 DevKit C
- 1 x red LED (LED 1)
- 1 x blue LED (LED 2)
- 2 x 220 ohm resistors (one per LED)

## Wiring

| From | To |
|------|----|
| GPIO 18 | Resistor R1 (220 ohm), then red LED anode |
| Red LED cathode | GND |
| GPIO 19 | Resistor R2 (220 ohm), then blue LED anode |
| Blue LED cathode | GND |

```
GPIO18 ---[220R]---|>|(red)---- GND
GPIO19 ---[220R]---|>|(blue)--- GND
```

## PWM frequencies

| | LED 1 (red) | LED 2 (blue) |
|---|---|---|
| GPIO pin | 18 | 19 |
| LEDC channel | 0 | 2 |
| Frequency | **1000 Hz** | **5000 Hz** |
| Resolution | 8 bit (0 to 255) | 8 bit (0 to 255) |

Channels 0 and 2 are used so each LED has its own LEDC timer. This lets the two LEDs run at different frequencies at the same time.

## Duty-cycle control

- Duty value `0` means off and `255` means fully on. Percentage = `duty / 255 x 100`.
- `ledcWrite(channel, duty)` sets the duty cycle.
- The `loop()` function does two steps forever:
  1. **Fade in:** duty goes from 0 to 255 in steps of 1, with a 10 ms delay per step (about 2.56 s).
  2. **Fade out:** duty goes from 255 to 0 in steps of 1, with a 10 ms delay per step (about 2.56 s).
- Both LEDs get the same duty value, so they fade together. Only their frequencies differ.
- One full cycle takes about 5.12 s.

## Serial output

At start-up the program prints the configured frequencies (115200 baud):

```
Dual LED PWM Fade Started
LED 1 Frequency: 1000.00 Hz
LED 2 Frequency: 5000.00 Hz
```

If PWM setup fails, it prints `ERROR: PWM setup failed.` and stops.

## How to run

**Wokwi:** open the project link and press the green Play button.

**PlatformIO (VS Code):** open the folder, click **Build**, then start the simulation with the Wokwi extension.

## Test notes

Expected behaviour when the simulation runs:

- Both LEDs start off, fade smoothly up to full brightness, then fade smoothly back to off.
- The cycle repeats continuously.
- The serial monitor shows 1000 Hz for LED 1 and 5000 Hz for LED 2.
- No flicker is visible at either frequency.
## Test results

**Test setup:** Wokwi simulation, ESP32 DevKit C, red LED on GPIO 18 (1 kHz) and blue LED on GPIO 19 (5 kHz). Serial monitor at 115200 baud.

**Serial monitor output:**

```
Dual LED PWM Fade Started
LED 1 Frequency: 1000.00 Hz
LED 2 Frequency: 5000.00 Hz
```

**Fade-in progress (10 ms per step):**

| Time (approx.) | Duty value | Duty cycle | LED brightness |
|----------------|-----------|------------|----------------|
| 0.00 s | 0 | 0 % | off |
| 0.64 s | 64 | 25 % | dim |
| 1.28 s | 128 | 50 % | medium |
| 1.92 s | 192 | 75 % | bright |
| 2.55 s | 255 | 100 % | full brightness |

After reaching 255, the duty value counts back down to 0 over another 2.55 s. One full fade-in and fade-out cycle takes about 5.12 s, then it repeats.

**Frequency comparison:**

| | LED 1 (red) | LED 2 (blue) |
|---|---|---|
| Frequency | 1000 Hz | 5000 Hz |
| PWM period | 1.0 ms | 0.2 ms |
| Duty step size (8-bit) | about 3.9 microseconds | about 0.78 microseconds |

**Summary of results:**

| Test | Result |
|------|--------|
| PWM setup succeeds for both channels | Pass (no error message printed) |
| Serial monitor shows 1000 Hz and 5000 Hz | Pass |
| Both LEDs fade in from off to full brightness | Pass |
| Both LEDs fade out from full brightness to off | Pass |
| Both LEDs fade together (same duty value) | Pass |
| Cycle repeats continuously | Pass |
| Visible flicker at 1 kHz or 5 kHz | None, both are far above what the eye can see |

**Conclusion:** Both LEDs faded smoothly with 256 brightness levels. The 1 kHz and 5 kHz frequencies looked the same to the eye, because both are well above the flicker threshold. The difference between them only matters for things like measuring the signal with an oscilloscope, or noise in other circuits.