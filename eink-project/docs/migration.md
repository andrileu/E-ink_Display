# 01 · Clean up the old local folder

**Goal:** one clear, VS Code-friendly local `eink-project` folder; no accidental loss of old notes.

## Safe migration (Windows)

1. **Back up** your existing `eink-project` folder as `eink-project-backup-2026-10-29`. If it is already a Git repository, keep its `.git/` directory in the backup.
2. Extract the **cleaned ZIP** to a convenient permanent location, e.g. `C:\Users\<you>\Documents\Projects\`. Its top-level folder is `eink-project/`.
3. **Do not extract directly on top of the old project**: rename or move the old directory first. Otherwise stale test folders and obsolete docs may remain.
4. If you have newer personal notes/photos on your PC (not in the submitted ZIP), copy those into `docs/images/` or `docs/LOG.md` in the new project. Do **not** copy the old `firmware/eink_hello/` sketch or obsolete pin tables back in.
5. Open **VS Code > File > Open Folder** and select the new `eink-project` directory (not just `docs` or a single `.ino`).
6. Open `README.md`, then press **Ctrl+Shift+V** for its formatted Markdown preview. Check that `docs/images/wiring-map.svg` appears.
7. Open [arduino-ide.md](arduino-ide.md) and record your actual current Tools settings. You do **not** need to change working IDE settings simply to match a recommendation.
8. Arduino IDE **File > Open**: choose [`01_full_refresh_v6.ino`](../firmware/01_full_refresh_v6/01_full_refresh_v6.ino), click **Verify**, then **Upload** to confirm the transferred files still work.
9. Add the outcome to `LOG.md`, then follow [Git setup](git.md).

## What changed from the submitted ZIP

| Old file / information | Action | Why |
| --- | --- | --- |
| `firmware/eink_hello/` | **Removed** | Obsolete BUSY7/RST8/DC9 mapping; not the tested firmware |
| `firmware/esp32_diagnostic/` | **Replaced** by `00_board_info` | Keep a small reusable board/serial utility |
| `docs/first-steps.md`, `docs/setup.md` | **Consolidated** | Now `migration.md`, `arduino-ide.md`, `git.md` |
| Old `TASKS.md` | **Removed** | Contained already-completed hardware milestones as unchecked tasks |
| Old `hardware.md` | **Rewritten** | Logical BUSY5/RST6/DC7 mapping is now tested |
| Original `LOG.md` | **Updated**, keeping original project origin | V6/V7 data recorded as actual results |
| V6/V7/V8 sketches from this chat | **Added** | Clear verified vs experimental distinction |
| `.vscode/settings.json` | **Added** | `.ino` syntax highlighting; Markdown/code editing defaults |

**Do not delete your backup immediately.** Keep it until the new sketch has uploaded, your documentation is intact, and your remote Git push succeeds.

> If the original local folder is already a Git repository with valuable history, do **not** initialize a second repo over it or delete `.git`. You can instead copy these cleaned files into the original repository, review deletions with `git status`, and commit the migration there.
