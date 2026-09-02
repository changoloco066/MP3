# Learning Notes — C++ / Embedded, coming from Java

Running log of concepts learned while building this project. Written for
future-me, coming from a Java/OOP background.

## PlatformIO / Board Config

- **N16R8 ≠ the stock `esp32-s3-devkitc-1` definition.** The stock board
  assumes N8 (8MB flash, no PSRAM). For N16R8 you need to manually set:
  - `board_build.flash_size = 16MB`
  - `board_build.partitions = default_16MB.csv` — flash size alone doesn't
    regenerate the partition table; you need this explicitly, or the
    build silently keeps using the old (smaller) partition layout.
  - `board_build.psram_type = opi`
  - `board_build.arduino.memory_type = qio_opi` (quad flash + octal PSRAM)
  - The `PLATFORM:` line in build logs will still print "N8, No PSRAM" —
    that's just a static label from the board definition, not the actual
    runtime config. What matters is the partition size in the build
    output and the `Embedded PSRAM 8MB` line at upload time.

- **`pio run -t clean`** doesn't always fully invalidate cached partition
  data — after changing flash/partition settings, clean *and* rebuild
  before trusting the numbers.

- **`pio` isn't in PATH** in a plain PowerShell window by default — use
  the PlatformIO toolbar buttons in VSCode, or the full path to
  `platformio.exe`.

## ESP32-S3 Upload Quirk

- After uploading via the native USB port, the chip often stays in
  `DOWNLOAD(USB/UART0)` mode (bootloader) instead of running the program.
  Press the physical **RESET** button once to boot normally
  (`SPI_FAST_FLASH_BOOT`).

## I2C Debugging

- `display.begin()` from Adafruit_SSD1306 can return `true` even when
  nothing is actually connected — don't trust it as a wiring check.
- **The reliable way to check I2C wiring:** a raw I2C scanner using
  `Wire.beginTransmission(address)` / `Wire.endTransmission()` across all
  addresses. If it finds nothing, the bus genuinely has no device
  responding — that's ground truth, unlike library-level "success".
- Unsoldered header pins pressed into a breadboard are a common cause of
  intermittent/failed I2C — the connection can look fine and even
  half-work, but isn't reliable enough for real communication.

## C++ vs Java

- **No `import`, no compile-time whole-file lookahead.** The compiler
  reads top to bottom in one pass. A type/enum used before it's declared
  will fail to compile — unlike Java, which resolves classes regardless
  of order. Declare things (enums, functions, global variables) *before*
  their first use, or forward-declare them.

- **`#include`** vs Java's `import` — `#include` is a literal textual
  copy-paste of the header file's contents at that point, done by the
  preprocessor before compilation even starts. That's part of why
  declaration order matters so much more in C++.

- **`#define`** is a preprocessor macro — plain text substitution before
  compilation, not a real typed constant. Used here for pin numbers and
  screen dimensions (`SCREEN_WIDTH`, `I2C_SDA`, etc.).

- **`enum class`** (vs plain `enum`): scopes its values under the type
  name, so you must write `PlayerState::PLAYING` instead of bare
  `PLAYING`. Prevents name collisions between different enums — closer
  in spirit to Java enums than C's old-style `enum`.

- **References (`&`)**: `Adafruit_SSD1306 display(..., &Wire, ...)` — the
  `&` passes the *address* of the `Wire` object instead of copying it.
  Conceptually similar to how Java objects are always passed by
  reference automatically; C++ makes this explicit.

- **Global objects without `new`**: `Adafruit_SSD1306 display(...)` at
  file scope directly constructs the object (stack/static allocation,
  no heap `new` needed) — different from Java where you'd always do
  `new Adafruit_SSD1306(...)`.

- **Classes typically split into two files:**
  - `.h` (header) — declarations only (what a class *has*)
  - `.cpp` — implementations (what it *does*)
  - `.h` files need an include guard (`#ifndef X_H / #define X_H / #endif`)
    to avoid duplicate-definition errors if the header gets included more
    than once — not an issue in Java, where the module system handles
    this differently.

## Adafruit_SSD1306 / GFX Cheat Sheet

- `display.clearDisplay()` — clears the in-memory buffer, not the
  physical screen yet.
- `display.display()` — the actual push to hardware. Forgetting this
  means nothing shows up even if everything else is correct.
- Screen resolution (128x64 vs 128x32) isn't always documented by cheap
  sellers — may need to test both if text renders wrong/blank despite a
  successful I2C connection.

## Soldering Confirmed the Diagnosis

After soldering the ESP32-S3 headers (instead of just pressing pins into
the breadboard), the OLED worked immediately with the exact same code
that failed before. Confirms the earlier I2C scanner diagnosis was
correct — it really was a physical contact issue the whole time, not
code. Lesson: for I2C specifically, don't trust "it mostly works" on
unsoldered header pins — the failure mode can look like a software bug
(e.g. `display.begin()` returning `true` with nothing connected) when
it's actually a flaky connection.

## Wokwi (abandoned for now)

- Tried to simulate the OLED wiring to avoid needing to solder headers
  right away. Got stuck on exact pin-naming conventions for the
  `board-esp32-s3-devkitc-1` part (e.g. `3V3` needed a `.1` suffix like
  `esp:3V3.1`, similar to how `GND` pins are suffixed `GND.1`, `GND.2`).
  The drag-and-drop wire editor requires a paid Wokwi membership.
  Decided it wasn't worth the friction — back to testing state/logic
  code via Serial Monitor on real hardware instead.