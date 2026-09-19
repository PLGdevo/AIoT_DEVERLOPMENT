#!/usr/bin/env bash
# SessionStart hook: injects a minimal, cheap project-state snapshot into the
# new session's context so Claude does not need to open every doc file just
# to know "where things stand". Reads git + markdown only, never touches
# application source code.
set -u
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$DIR/lib.sh"

# Consume stdin (hook input JSON) without needing a JSON parser.
cat >/dev/null 2>&1 || true

ROOT="$(project_root)"
cd "$ROOT" || exit 0

GIT_LINE="$(git_snapshot)"
NEXT_ACTIONS="$(extract_section "$ROOT/PROJECT_STATE.md" "Next Actions" | head -n 3)"
IN_PROGRESS="$(extract_section "$ROOT/PROJECT_STATE.md" "In Progress" | head -n 3)"
TODO_IN_PROGRESS="$(extract_section "$ROOT/TODO.md" "In Progress" | head -n 5)"

[ -z "$NEXT_ACTIONS" ] && NEXT_ACTIONS="(none listed)"
[ -z "$IN_PROGRESS" ] && IN_PROGRESS="(none)"
[ -z "$TODO_IN_PROGRESS" ] && TODO_IN_PROGRESS="(none)"

CONTEXT_TEXT="[context-manager] Project snapshot (auto-injected, do not re-derive from scratch):
Git: ${GIT_LINE}
PROJECT_STATE.md > In Progress: ${IN_PROGRESS}
PROJECT_STATE.md > Next Actions: ${NEXT_ACTIONS}
TODO.md > In Progress: ${TODO_IN_PROGRESS}
Full docs available on demand: ARCHITECTURE.md, HARDWARE.md, DECISIONS.md, TODO.md, SESSION_SUMMARY.md (see CLAUDE.md load-tier table before opening them)."

PYBIN="$(command -v python3 || command -v python)"
"$PYBIN" - "$CONTEXT_TEXT" <<'PYEOF'
import json, sys
text = sys.argv[1]
print(json.dumps({
    "hookSpecificOutput": {
        "hookEventName": "SessionStart",
        "additionalContext": text
    }
}))
PYEOF
