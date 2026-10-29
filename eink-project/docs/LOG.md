# Test history and decisions

Only record **observed** test results here. Keep this concise; detailed procedure lives in [`tests.md`](tests.md).

## 2026-10-21 — Project preparation (from original ZIP)

- Hardware planned: ESP32-S3 N16R8 + Good Display GDEY075T7 + DESPI-C02.
- Workflow chosen: VS Code docs/source; Arduino IDE 2 flash/serial; Git for snapshots.
- **Obsolete** initial proposal: BUSY7/RST8/DC9, CS10/MOSI11/SCK12. **Do not use.**

## 2026-10-29 — Hardware bring-up

- Early fixed-delay screen test printed completion but the display did **not** visibly refresh.
- New board physical pin layout did not match the generic DevKitC pinout reference.
- After fixing wiring, a first successful full refresh gave:

  ```text
  _PowerOn : 125997
  _Update_Full : 1542001
  _PowerOff : 40001
  ```

- **Verified logical GPIOs:** BUSY5/RST6/DC7/CS10/MOSI11/SCK12. Physical ESP32 board model still unknown.

## 2026-10-29 — V6 full-screen test — PASS

- Both drawn patterns appeared correctly.
- `_Update_Full` = **1,542,001 µs** for both.
- Overall pattern durations **2,685 ms** and **2,119 ms**.
- BUSY transitions: 9 then 15 cumulative.

## 2026-10-29 — V7 center-window partial test — PASS

- Both capabilities reported `YES`: `hasPartialUpdate`, `hasFastPartialUpdate`.
- Counter 0 → 4 appeared correctly. User observed **no ghosting**.
- `_Update_Part` = **424,001 µs**; overall update durations **621, 493, 493, 493 ms**.
- Initial full refresh **2,657 ms**; final full refresh **1,964 ms**.

## 2026-10-29 — V8 multi-region test — PARTIAL

- User reported **"Worked approximately well."**
- Keep as experimental until the specific visual issues and timings are captured. Do not claim all five independent region updates passed perfectly.

## 2026-10-29 — Source/documentation cleanup

- Replaced obsolete firmware and out-of-date GPIO docs; organized tests by purpose/status.
- Bundled verified V6 and V7 sketches plus experimental V8 and board-info utility.
- Added Arduino IDE settings sheet, logical wiring graphic, migration instructions and Git guide.
- **Not yet done on local PC:** extraction, local compilation of rebuilt sketches, first Git commit, remote push.


## 2026-10-29 — Arduino settings screenshot and Git setup blocker

- Working Arduino IDE Tools screenshot archived at `docs/images/arduino-tools-2026-10-29.png`; all selected values transcribed into `arduino-ide.md`.
- Confirmed `ESP32S3 Dev Module`, `COM3`, `USB CDC On Boot = Enabled`, `Flash Mode = QIO 80MHz`, `Flash Size = 16MB (128Mb)`, `PSRAM = OPI PSRAM`, `Upload Mode = UART0 / Hardware CDC`, `Upload Speed = 921600`, `USB Mode = Hardware CDC and JTAG`.
- **Partition Scheme** is `Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)` despite 16 MB physical flash. Keep current known-good configuration until future requirements justify changing it.
- Windows VS Code PowerShell reported **`git` not recognized**. Git installation and initial remote push are **not completed**. See updated `git.md` for the install/PATH and first-push steps.

## Template for your next test

```text
YYYY-MM-DD — Sketch / version:
Result: PASS / PARTIAL / FAIL
Arduino IDE / ESP32 core / GxEPD2 versions:
USB port / power supply:
Settings changed (if any):
Display behavior:
Relevant Serial output:
Git commit hash (if committed):
Next action:
```
