# 04 · Firmware test catalog

Use a known-good sketch when changing hardware or software. **Each test is self-contained**: one sketch folder, one same-named `.ino` file, a header describing how to run it.

| Test | On-screen behavior | Key evidence | State |
| --- | --- | --- | --- |
| **00 Board info** | No screen update; serial heartbeat | USB output, flash / PSRAM | Utility |
| **01 Full V6** | Four blocks -> bars + text | Two patterns displayed; `_Update_Full = 1542001` µs | ✅ PASS |
| **02 Partial V7** | Counter 0 → 4; other areas remain | `_Update_Part = 424001` µs; no ghosting | ✅ PASS |
| **03 Regions V8** | Number, status, bar and 32×32 flag change separately | User reported "worked approximately well" | 🟡 Refine |

## How to run a firmware test

1. Keep the verified hardware wires unchanged ([hardware.md](hardware.md)).
2. Arduino IDE → **File → Open** → choose the `.ino` in the matching `firmware/` folder.
3. Tools → **ESP32S3 Dev Module** and the current COM port; check [Arduino settings](arduino-ide.md).
4. **Verify** → **Upload** → Serial Monitor at **115200 baud**.
5. Observe the screen and capture the *new* serial section beginning with its `=== TEST ... ===` heading. Clear old Serial Monitor text to avoid confusing logs.
6. Record results in [`LOG.md`](LOG.md); Git commit after changes are confirmed.

## V6 — full refresh reference

**Actual outcome on 2026-10-29:** both distinct patterns appeared correctly.

| Metric | Pattern 1 | Pattern 2 |
| --- | ---: | ---: |
| `_PowerOn` | 125,999 µs | 126,001 µs |
| `_Update_Full` | 1,542,001 µs | 1,542,001 µs |
| `_PowerOff` | 40,001 µs | 40,001 µs |
| Whole sketch drawing/refresh call | 2,685 ms | 2,119 ms |
| Cumulative BUSY transitions | 9 | 15 |

## V7 — partial refresh reference

**Actual outcome on 2026-10-29:** counter updated correctly from 0 to 4, static area stayed unchanged, no visible ghosting.

| Stage | Duration including sketch overhead |
| --- | ---: |
| Initial full refresh | 2,657 ms |
| First partial refresh | 621 ms |
| Partial 2, 3, 4 | 493 ms each |
| Final full refresh | 1,964 ms |
| Controller full waveform (`_Update_Full`) | **1,542,001 µs** |
| Controller partial waveform (`_Update_Part`) | **424,001 µs** |

**Interpretation:** partial controller update took about **3.6× less time** than a full controller update in this test. A smaller update rectangle does not necessarily shorten the panel waveform; rendering/SPI overhead is additional.

## V8 — multi-region experimental

Five partial operations update value, status, progress, then a 32 × 32 flag from white → black → white; a full redraw follows. **Not yet formally passed** because the user reported only approximate visual success. Avoid treating it as the finished dashboard. Note which specific pixels, fonts, borders or transitions were imperfect before modifying it.

## New tests — naming convention

Add a directory and matching sketch basename, e.g. `firmware/04_sleep_wake/04_sleep_wake.ino`.

Start every sketch with a comment block containing **STATUS, PURPOSE, EXPECTED SCREEN, SERIAL BAUD, LIBRARIES, PIN MAP, and SAFETY/NOTES**. Give it a unique serial heading; perform bounded tests (not endless rapid refresh loops); document measured results in `LOG.md`.
