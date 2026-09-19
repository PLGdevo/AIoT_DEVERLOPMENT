---
description: Load minimum context, check git state, identify current task, report briefly.
---

Invoke the `context-manager` skill in **start-session** mode (`Skill({ skill: "context-manager", args: "start-session" })`) and follow its instructions exactly. Do not read every doc file up front — the skill tells you what's already been injected and what's still worth reading.

Report back in under 5 lines: branch/HEAD, current task (if any), next action, blockers (if any).
