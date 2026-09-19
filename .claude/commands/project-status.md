---
description: Read-only report of current task, blockers, and next actions. Never edits files.
---

Invoke the `context-manager` skill in **project-status** mode (`Skill({ skill: "context-manager", args: "project-status" })`) and follow its instructions exactly.

This is read-only — do not edit `PROJECT_STATE.md`, `TODO.md`, or any other file. Cross-check against `git status --short` and flag if uncommitted work isn't reflected in the docs. Report current task, blockers, next actions.
