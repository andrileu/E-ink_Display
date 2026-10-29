# 06 · Git setup and first GitHub push — Windows / VS Code

**Your local project folder (from 29 October 2026 terminal):**

```text
C:\Users\User\Documents\02_Projects\01_E-ink\E-ink_Dashboard_Project
```

This folder can keep its current name — it **does not** need to be called `eink-project`. Open it in VS Code with **File → Open Folder**. Make sure it contains `README.md`, `docs/`, and `firmware/` (not an extra nested `eink-project` folder).

> **Current blocker:** PowerShell reported `git : The term 'git' is not recognized`. Install Git for Windows (or fix PATH) first; this is not an E-Ink firmware error.

## A · Install and verify Git (one time)

1. Download [Git for Windows](https://git-scm.com/install/windows) (standard installer, not portable edition).
2. In setup, select **Git from the command line and also from 3rd-party software** for the PATH environment option. Keep other defaults unless you have a preference.
3. **Fully close VS Code** and re-open it, then open a **new** integrated PowerShell terminal.
4. Run:

   ```powershell
   git --version
   ```

   A Git version number means this prerequisite is complete. VS Code uses the system Git installation.

If Git is still not found:

```powershell
& "C:\Program Files\Git\cmd\git.exe" --version
```

If this shows a version, Git is installed but not on PATH for this terminal. Restart VS Code/Windows or rerun the Git installer with the recommended PATH option. If that file also does not exist, check where Git was installed. Don't change firmware settings to solve a Git installation problem.

## B · Check your exact folder and whether Git is already initialized

In VS Code Terminal → New Terminal:

```powershell
pwd
git status
```

`pwd` should show your `E-ink_Dashboard_Project` folder. If `git status` says **not a git repository**, run once:

```powershell
git init -b main
```

If Git already recognizes the folder as a repository, **do not initialize it again** or delete `.git`. Preserve its history.

## C · Set your author identity, then create first commit

Enter your own name and preferred commit email (not a password):

```powershell
git config --global user.name "Your Name"
git config --global user.email "your-email@example.com"
```

Then inspect and commit your project files:

```powershell
git status
git add .
git diff --cached --stat
git commit -m "Document verified ESP32-S3 E-Ink baseline"
```

Before committing, inspect the staged file summary. Do not commit Wi-Fi passwords, access tokens, personal files, or generated build directories. If Git says *nothing to commit*, there may already be a valid commit — check `git log -1 --oneline`.

## D · Create an EMPTY repository on GitHub

1. Sign in at [GitHub → New repository](https://github.com/new).
2. Suggested repository name: `E-ink_Dashboard_Project` (or `eink-project` if you prefer). Private is a sensible default.
3. **Do not** check the new README, .gitignore, or license options: you already have local files.
4. Click Create, then **copy the exact HTTPS URL** GitHub gives you. Do not literally paste `YOUR_USERNAME` or `YOUR_GITHUB_USERNAME` from an example.

For example only, a repository URL has this shape:

```text
https://github.com/ACTUAL_GITHUB_ACCOUNT/E-ink_Dashboard_Project.git
```

## E · Connect and push

From the local VS Code terminal:

```powershell
git branch --show-current
git remote -v
```

If you have **no** `origin`, add one using your **real copied GitHub URL**:

```powershell
git remote add origin https://github.com/ACTUAL_GITHUB_ACCOUNT/E-ink_Dashboard_Project.git
```

**Replace `ACTUAL_GITHUB_ACCOUNT` and repository name with your real values.** If `git remote -v` already shows `origin`, do not add it again. Check whether it's correct; if not, use `git remote set-url origin <the-actual-copied-HTTPS-url>`.

Then push. If the local branch is `main`:

```powershell
git push -u origin main
```

If the branch has a different name, use that exact name instead of `main`. A browser sign-in prompt from Git Credential Manager is normal. Never paste your GitHub password into a Git remote URL or your source files.

**Verify:** Refresh the GitHub page. You should see `README.md`, `docs/`, and `firmware/`.

## F · Everyday workflow

After editing in VS Code, flashing in Arduino IDE, and writing down the test result:

```powershell
git status
git add firmware docs README.md
git commit -m "Record E-Ink display test results"
git push
```

Or use VS Code's **Source Control** sidebar to stage, commit, and sync. Git pushes source files to GitHub — **it does not flash the ESP32**.

## Error reference

| Error | Action |
| --- | --- |
| `git` not recognized | Install Git for Windows; close/reopen VS Code; retry `git --version`; verify path as above |
| `not a git repository` | Confirm `pwd`; initialize in the project root once |
| `remote origin already exists` | `git remote -v`; inspect existing remote and use `git remote set-url` if incorrect |
| `Repository not found` | Replace example username/repo with the actual GitHub HTTPS URL; check permissions |
| `src refspec main does not match any` | Make a first commit and check `git branch --show-current` |
| `nothing to commit` | No new changes; check `git status` / `git log -1` |
| GitHub login fails | Use the browser credential manager flow; check account access |
| Push rejected (remote has commits) | Inspect divergence; **do not force-push** blindly |

Official references: [Git for Windows](https://git-scm.com/install/windows) · [VS Code: Git not found](https://code.visualstudio.com/docs/sourcecontrol/troubleshooting) · [VS Code source control](https://code.visualstudio.com/docs/sourcecontrol/overview) · [GitHub: add an existing project](https://docs.github.com/en/migrations/importing-source-code/using-the-command-line-to-import-source-code/adding-locally-hosted-code-to-github).
