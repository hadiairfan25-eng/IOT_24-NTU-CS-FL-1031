# Homework 2 - Task 2: Two-LED PWM Fading with SSD1306 OLED (ESP32)

This is the completed project. It extends Task 1 by adding an SSD1306 OLED display. Two LEDs fade in and out using ESP32 hardware PWM at two different frequencies, and the OLED shows each LED's **live duty-cycle percentage** and its **configured PWM frequency**.

- **Author:** Hadia
- **Board:** ESP32 DevKit C (Wokwi: `board-esp32-devkit-c-v4`, PlatformIO: `esp32dev`)
- **Framework:** Arduino (PlatformIO)
- **Libraries:** Adafruit GFX Library, Adafruit SSD1306
- **Wokwi project link:** _add your link here_

## Project files

```
Task2_pwm-OLED/
├── src/
│   └── main.cpp        # final source code
├── diagram.json        # Wokwi circuit configuration (with OLED)
├── wokwi.toml          # Wokwi firmware paths
├── platformio.ini      # PlatformIO settings and library dependencies
├── images/             # screenshots and hardware photographs
└── README.md
```

## Components

| Qty | Component | Notes |
|-----|-----------|-------|
| 1 | ESP32 DevKit C | main board |
| 1 | Red LED | LED 1 |
| 1 | Blue LED | LED 2 |
| 2 | 220 ohm resistor | current limiting, one per LED |
| 1 | SSD1306 OLED, 128x64, I2C | address `0x3C` |

## Wiring

**LEDs**

| ESP32 pin | Connects to |
|-----------|-------------|
| GPIO 18 | Red LED anode (LED 1) |
| Red LED cathode | 220 ohm resistor (R1), then GND |
| GPIO 19 | Blue LED anode (LED 2) |
| Blue LED cathode | 220 ohm resistor (R2), then GND |

**OLED (I2C)**

| OLED pin | ESP32 pin |
|----------|-----------|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

```
GPIO18 ---|>|(red)---[220R]--- GND
GPIO19 ---|>|(blue)--[220R]--- GND

3V3    ---- OLED VCC
GND    ---- OLED GND
GPIO21 ---- OLED SDA
GPIO22 ---- OLED SCL
```

## PWM frequencies

| Setting | LED 1 (red) | LED 2 (blue) |
|---------|-------------|--------------|
| GPIO pin | 18 | 19 |
| LEDC channel | 0 | 2 |
| PWM frequency | **1000 Hz (1 kHz)** | **5000 Hz (5 kHz)** |
| Resolution | 8 bit | 8 bit |
| Duty range | 0 to 255 | 0 to 255 |

Channels 0 and 2 are used so that each LED gets its own LEDC timer. This is what allows the two LEDs to run at different frequencies at the same time.

## Duty-cycle control

- The duty value (`duty1`, `duty2`) goes from `0` (off) to `255` (fully on).
- `ledcWrite(channel, duty)` applies the duty value to the PWM channel.
- In `loop()`:
  1. **Fade in:** duty rises from 0 to 255 in steps of 1.
  2. **Fade out:** duty falls from 255 to 0 in steps of 1.
  3. After every step, the OLED is refreshed and the code waits 10 ms.
- Both LEDs use the same duty value (`duty2 = duty1`), so they fade together. Only their PWM frequency is different.
- The percentage shown on the OLED is calculated as `duty * 100 / 255`.

## OLED output

The display is 128x64 pixels with 1x text size. It is updated by `showOLED()` on every duty step, and shows three lines:

```
PWM MONITOR
LED1: 50% F:1000Hz
LED2: 50% F:5000Hz
```

- The percentage values change live from 0 % to 100 % and back as the LEDs fade.
- The frequencies stay fixed at 1000 Hz and 5000 Hz.
- If the OLED is not found at address `0x3C`, the serial monitor prints `OLED initialization failed!` and the program stops.

**Serial monitor at start-up (115200 baud):**

```
Task 2: Dual LED PWM with OLED
```

## How to run

**Wokwi:** open the project link above and press the green Play button.

**PlatformIO (VS Code):**
1. Open the project folder. PlatformIO downloads the two Adafruit libraries automatically from `platformio.ini`.
2. Click **Build**.
3. Start the simulation using the Wokwi extension (`diagram.json`), or upload to a real ESP32 board with **Upload**.


## Test results

**Test setup:** ESP32 DevKit C, red LED on GPIO 18 (1 kHz), blue LED on GPIO 19 (5 kHz), SSD1306 OLED (128x64, I2C address 0x3C) on GPIO 21 (SDA) and GPIO 22 (SCL). Tested first in Wokwi, then on real hardware. Serial monitor at 115200 baud.

**Serial monitor output:**

```
Task 2: Dual LED PWM with OLED
```

**OLED readings during the fade-in:**

| Duty value | OLED line 2 | OLED line 3 | LED brightness |
|-----------|-------------|-------------|----------------|
| 0 | `LED1: 0% F:1000Hz` | `LED2: 0% F:5000Hz` | off |
| 64 | `LED1: 25% F:1000Hz` | `LED2: 25% F:5000Hz` | dim |
| 128 | `LED1: 50% F:1000Hz` | `LED2: 50% F:5000Hz` | medium |
| 192 | `LED1: 75% F:1000Hz` | `LED2: 75% F:5000Hz` | bright |
| 255 | `LED1: 100% F:1000Hz` | `LED2: 100% F:5000Hz` | full brightness |

The first OLED line always shows `PWM MONITOR`. During the fade-out the percentages count back down from 100 % to 0 %, and the frequencies stay at 1000 Hz and 5000 Hz.

**Summary of results:**

| Test | Wokwi | Hardware |
|------|-------|----------|
| OLED initializes and shows "PWM MONITOR" | Pass | Pass |
| LED 1 fades in and out at 1000 Hz | Pass | Pass |
| LED 2 fades in and out at 5000 Hz | Pass | Pass |
| OLED percentage follows LED brightness (0 % to 100 % and back) | Pass | Pass |
| OLED shows F:1000Hz and F:5000Hz | Pass | Pass |
| Both LEDs fade together | Pass | Pass |
| Cycle repeats continuously | Pass | Pass |
| Visible flicker at 1 kHz or 5 kHz | None | None |
| No "OLED initialization failed!" message | Pass | Pass |

**Fade speed:** The code waits 10 ms per step, and 512 steps make one cycle (5.12 s if there were no other delay). The OLED is redrawn on every step, and each redraw takes extra time, so the real cycle is longer, roughly 15 to 20 seconds.

## Wokwi vs hardware comparison

The Wokwi simulation and the real hardware gave the same results. In both, the red and blue LEDs faded in and out together, and the OLED showed the live duty-cycle percentage and the fixed frequencies (1000 Hz and 5000 Hz) correctly. No flicker could be seen at either frequency.

The small differences were in the physical details. On the real board, the LED brightness looked slightly different between the red and blue LEDs, because the two colors have different forward voltages and brightness. The OLED text on hardware was small and needed a close look, while in Wokwi it was easy to read on screen. The fade speed was slower than the 10 ms per step in the code in both cases, because redrawing the OLED on every step adds delay. Overall, Wokwi was a reliable way to test the code before wiring the real circuit.
