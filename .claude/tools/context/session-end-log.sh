#!/usr/bin/env bash
# SessionEnd hook: pure bookkeeping. Cannot invoke the model, so it cannot
# write SESSION_SUMMARY.md or update PROJECT_STATE.md/TODO.md itself — that
# needs judgment, which is what /end-session (run by Claude, before the
# session actually ends) is for. This just logs a fact-based line so the
# NEXT SessionStart can tell whether the previous session ended cleanly
# (i.e. whether /end-session was run) or was left dangling. Never touches
# application source code.
set -u
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$DIR/lib.sh"

INPUT="$(cat 2>/dev/null || true)"

ROOT="$(project_root)"
CACHE_DIR="$ROOT/.claude/tools/context/.cache"
mkdir -p "$CACHE_DIR"
LOG_FILE="$CACHE_DIR/session-log.txt"

cd "$ROOT" || exit 0

REASON="unknown"
PYBIN="$(command -v python3 || command -v python || true)"
if [ -n "$PYBIN" ] && [ -n "$INPUT" ]; then
    REASON="$(printf '%s' "$INPUT" | "$PYBIN" -c 'import json,sys
try:
    d=json.load(sys.stdin)
    print(d.get("reason","unknown"))
except Exception:
    print("unknown")' 2>/dev/null || echo unknown)"
fi

DIRTY_COUNT="$(git status --porcelain 2>/dev/null | wc -l | tr -d ' ')"
HEAD="$(git rev-parse --short HEAD 2>/dev/null || echo '(none)')"
TIMESTAMP="$(date -u +%Y-%m-%dT%H:%M:%SZ)"

# Was /end-session run recently (SESSION_SUMMARY.md touched after last checkpoint marker)?
END_SESSION_RAN="no"
if [ -f "$ROOT/SESSION_SUMMARY.md" ]; then
    summary_age=$(( $(date +%s) - $(date -r "$ROOT/SESSION_SUMMARY.md" +%s 2>/dev/null || echo 0) ))
    [ "$summary_age" -lt 1800 ] && END_SESSION_RAN="yes (SESSION_SUMMARY.md updated <30min ago)"
fi

echo "${TIMESTAMP} reason=${REASON} head=${HEAD} dirty=${DIRTY_COUNT} end_session_ran=${END_SESSION_RAN}" >> "$LOG_FILE"

exit 0
