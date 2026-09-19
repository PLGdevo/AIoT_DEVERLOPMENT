---
name: context-manager
description: Maintains this project's cross-session context files (PROJECT_STATE.md, TODO.md, DECISIONS.md, SESSION_SUMMARY.md). Invoked by /start-session, /checkpoint, /end-session, /project-status. Never copies conversation history — extracts only what a future session needs from git/code/build state.
---

# Context Manager

Maintains the context-management doc set for this repo:
`CLAUDE.md`, `PROJECT_STATE.md`, `ARCHITECTURE.md`, `HARDWARE.md`, `DECISIONS.md`, `TODO.md`, `SESSION_SUMMARY.md`.

This skill is invoked with an argument: `start-session`, `checkpoint`, `end-session`, or `project-status`. Jump to the matching section.

## Hard rules (apply to every mode)

1. **Never copy conversation transcript into any file.** Extract only decisions/state a future session needs — not how the current conversation arrived there.
2. **Ground every claim in something checkable**, in this priority order: current source code > `git status`/`git diff` > build/test output > existing config files > prior doc content > conversation assumptions. If the conversation claimed something that current `git diff`/code contradicts, trust the code and flag the discrepancy — don't silently repeat the stale claim.
3. **Never touch application source code** (`src/`, `examples/`, `tools/*.py`, `platformio.ini`, etc.) as part of these commands. Only the doc files listed above, plus `.claude/tools/context/.cache/*` markers.
4. **Don't create new documentation files.** Update the existing seven. If something doesn't fit any of them, it probably doesn't need to be written down.
5. **Keep edits surgical.** Use targeted edits to the relevant section, not full-file rewrites, unless the section has genuinely outgrown its structure.
6. **Section headers in `PROJECT_STATE.md` and `TODO.md` stay in English** (`## In Progress`, `## Next Actions`, `## Blockers`, `## Pending`, `## Blocked`) — the `SessionStart`/`Stop` hooks grep these literal strings. Content under them can be Vietnamese or English.

## Mode: start-session

Goal: minimum-context orientation, not a full re-read of every doc.

1. The `SessionStart` hook has likely already injected a snapshot (git branch/HEAD/dirty count + `PROJECT_STATE.md`'s "In Progress"/"Next Actions" + `TODO.md`'s "In Progress") into context via `additionalContext`. Check if that's already present before re-reading files.
2. If it's missing or you need more: run `git status --short` and `git log -3 --oneline` yourself, and read only `PROJECT_STATE.md` (short file, always relevant).
3. Do **not** proactively read `ARCHITECTURE.md`, `HARDWARE.md`, `DECISIONS.md`, `SESSION_SUMMARY.md` at this point — only when the task at hand actually needs them (see the load-tier table in `CLAUDE.md`).
4. Report back to the user in under 5 lines: current branch/HEAD, what's in progress (if anything), what the next action is, and any blocker. No preamble, no re-explaining the whole project.

## Mode: checkpoint

Goal: cheap mid-session save point. Triggered manually or by the `Stop` hook's reminder.

1. Run `git status --short` and `git diff --stat` — this is the ground truth for "what changed," not what you remember discussing.
2. Update `PROJECT_STATE.md`:
   - Move finished items from "In Progress" to a one-line mention (full detail goes to `SESSION_SUMMARY.md` at end-session, not here).
   - Update "Next Actions" if the immediate next step changed.
3. Update `TODO.md` if any pending item was completed, started, or a new one surfaced — move between `In Progress`/`Pending`/`Blocked` sections; don't rewrite the whole file.
4. Update `DECISIONS.md` **only if** a non-obvious technical decision was actually made this session (new entry `D-NNN` following the existing format: Quyết định / Lý do / Phương án khác / Hệ quả). Most checkpoints won't need this — don't force an entry.
5. Touch the checkpoint marker so the `Stop` hook's cooldown resets: `date > .claude/tools/context/.cache/last-checkpoint` (create the `.cache` dir if missing; this file is gitignored).
6. Do NOT write `SESSION_SUMMARY.md` here — that's `/end-session`'s job. Do NOT create any new file.
7. Report what was updated in 1-2 lines. No essay.

## Mode: end-session

Goal: leave the repo in a state where a brand-new session can pick up cold.

1. Inspect ground truth first: `git status --short`, `git diff --stat`, and `git log` since the session's starting commit (if known) or since the last `SESSION_SUMMARY.md` timestamp.
2. Update `PROJECT_STATE.md`: Completed / In Progress / Blockers / Next Actions, reflecting the real end state — not what was planned at session start.
3. Update `TODO.md`: remove items actually finished (they graduate into `SESSION_SUMMARY.md`'s "Công việc đã hoàn thành" instead of lingering in TODO), add anything newly surfaced, correct any that turned out blocked.
4. Update `DECISIONS.md` if a real technical decision was made and not yet recorded (see checkpoint mode's guidance — don't force it).
5. **Overwrite** `SESSION_SUMMARY.md` (it holds only the latest session, not a history) with: date, session goal, files changed (from `git diff --name-status`), what was completed, test/build results if any were run, remaining issues, and the concrete next action. Keep it under ~40 lines — this is a handoff note, not a transcript.
6. Do not touch application source code, `.claude/tools/context/.cache/*` snapshots are fine to leave as-is (informational only).
7. Report a short summary to the user (what got updated), then stop — don't re-print the full SESSION_SUMMARY.md content back into chat unless asked.

## Mode: project-status

Goal: read-only status report. **Never edit any file in this mode.**

1. Read `PROJECT_STATE.md` (Snapshot/Completed/In Progress/Blockers/Next Actions) and `TODO.md`'s In Progress/Pending/Blocked sections.
2. Cross-check against `git status --short` — if there's uncommitted work not reflected in `PROJECT_STATE.md`, say so explicitly rather than silently trusting the stale doc.
3. Report: current task, blockers (if any), next actions. Keep it to what's asked — this is a status check, not an invitation to start fixing things.
