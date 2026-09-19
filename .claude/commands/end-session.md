---
description: Close out the session - inspect git diff/status, update state docs, write SESSION_SUMMARY.md.
---

Invoke the `context-manager` skill in **end-session** mode (`Skill({ skill: "context-manager", args: "end-session" })`) and follow its instructions exactly.

Inspect `git status`/`git diff`/`git log` for what actually happened this session (not what was planned). Update `PROJECT_STATE.md` and `TODO.md`, update `DECISIONS.md` only if a real decision was made, and overwrite `SESSION_SUMMARY.md` with a concise handoff note (it holds only the latest session). Do not touch application source code. Report a short summary of what was updated.
