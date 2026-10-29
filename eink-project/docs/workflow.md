# 05 · Everyday development workflow

```text
  VS Code (.ino + Markdown)
           │ save Ctrl+S
           ▼
  Arduino IDE (open SAME .ino)
           │ Verify → Upload → Serial Monitor
           ▼
  Observe screen + copy timing output
           │ update docs/LOG.md
           ▼
  VS Code Source Control / Git commit → push
```

1. **Open root folder** `eink-project/` in VS Code. Keep Arduino IDE on the same `.ino` file path — **no Save As duplicate**.
2. **Change one thing at a time.** For risky changes, copy a known-good test into a **new** folder with matching `.ino` filename. Do not overwrite V6/V7 reference tests unnecessarily.
3. **Save** in VS Code, **Verify and Upload** using Arduino IDE, set Serial Monitor to 115200 baud, observe the display, and note actual result.
4. Add a dated line to `docs/LOG.md` and update test status if needed.
5. Commit the relevant sketch and docs; **push** after you have a connected remote (`docs/git.md`).

### Frequent Git commands (VS Code Terminal / PowerShell)

```powershell
git status
git diff
git add firmware docs README.md
git commit -m "Document partial refresh test"
git push
```

Use a truthful commit message reflecting what actually changed. `git push` **does not flash the ESP32**. If the same file is open in both VS Code and Arduino IDE, do not leave unsaved conflicting edits. Keep credentials and Wi-Fi passwords out of Git; the repository ignores common secret filenames.

### Project phase boundary

This repository is now the **hardware/firmware baseline**. The next, separate conversation can design and implement the dashboard using the existing known-good panel driver and pin map.
