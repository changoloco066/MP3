# MP3 Player — ESP32-S3

A DIY MP3 player built on an ESP32-S3 N16R8, learning embedded C++ and
PlatformIO from the ground up.

## Hardware

| Component | Status |
|---|---|
| ESP32-S3-DevKitC-1 (N16R8: 16MB flash, 8MB PSRAM) | ✅ Working |
| 0.96" I2C OLED (SSD1306, 128x64 assumed) | ⚠️ Wired but unreliable — header pins not soldered yet |
| microSD module (SPI) | 🔲 Not acquired |
| MAX98357A I2S DAC + amp | 🔲 Not acquired |
| Small speaker (4-8Ω, 3W) | 🔲 Not acquired |
| Physical buttons (play/pause/next/prev) | 🔲 Not acquired |

## Pinout (current)

| Signal | GPIO |
|---|---|
| I2C SDA (OLED) | GPIO8 |
| I2C SCL (OLED) | GPIO9 |

## Project Status

Currently building the player logic (state machine + fake playlist) using
the Serial Monitor as the interface, since the OLED headers aren't
soldered yet and connections are unreliable on the breadboard. Once the
OLED is soldered and the SD card / DAC arrive, the plan is to wire this
logic into the real display and real audio output.

## Toolchain

- **Framework:** Arduino (via PlatformIO)
- **IDE:** VSCode + PlatformIO extension
- **Board config:** `esp32-s3-devkitc-1` with manual flash/PSRAM overrides
  for the N16R8 variant (16MB flash / 8MB octal PSRAM) — the stock board
  definition assumes N8 with no PSRAM, so `platformio.ini` overrides
  `board_build.flash_size`, `board_build.partitions`,
  `board_build.psram_type`, and `board_build.arduino.memory_type`.

## Architecture Plan

1. **Storage layer** — read `.mp3` files from SD card (`SD.h`, SPI)
2. **Decode/output layer** — `ESP8266Audio` library
   (`AudioFileSourceSD`, `AudioGeneratorMP3`, `AudioOutputI2S`)
3. **UI layer** — OLED showing current track/state, physical buttons
4. **Control layer** — state machine (Stopped / Playing / Paused) tying
   the above together

Currently working on step 4 in isolation, using a hardcoded fake playlist
and Serial commands instead of real files/buttons.

## Notes

See `notes.md` for a running log of C++/embedded concepts learned while
building this (useful since this is a C++ beginner project coming from a
Java background).