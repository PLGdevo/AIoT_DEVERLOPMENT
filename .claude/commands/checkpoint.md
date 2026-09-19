---
description: Save a mid-session checkpoint - update PROJECT_STATE.md/TODO.md/DECISIONS.md from actual git/code state.
---

Invoke the `context-manager` skill in **checkpoint** mode (`Skill({ skill: "context-manager", args: "checkpoint" })`) and follow its instructions exactly.

Ground the update in `git status`/`git diff`, not in what you recall discussing. Do not generate unnecessary documentation, and do not write `SESSION_SUMMARY.md` here. Report what was updated in 1-2 lines.
