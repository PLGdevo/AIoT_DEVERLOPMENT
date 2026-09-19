#!/usr/bin/env bash
# Stop hook: fires after every assistant turn, but stays silent (no output,
# no token cost) unless a real threshold is crossed. Only then does it emit
# a short systemMessage suggesting /checkpoint. Cooldown prevents nagging on
# every single turn once the threshold is crossed. Never touches app source.
set -u
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$DIR/lib.sh"

cat >/dev/null 2>&1 || true

ROOT="$(project_root)"
cd "$ROOT" || exit 0

if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    exit 0
fi

DIRTY_COUNT="$(git status --porcelain 2>/dev/null | wc -l | tr -d ' ')"
[ "$DIRTY_COUNT" -eq 0 ] && exit 0

CACHE_DIR="$ROOT/.claude/tools/context/.cache"
mkdir -p "$CACHE_DIR"
LAST_CHECKPOINT="$CACHE_DIR/last-checkpoint"
LAST_REMINDER="$CACHE_DIR/last-reminder"
NOW=$(date +%s)

checkpoint_age=999999
[ -f "$LAST_CHECKPOINT" ] && checkpoint_age=$(( NOW - $(date -r "$LAST_CHECKPOINT" +%s 2>/dev/null || echo "$NOW") ))

reminder_age=999999
[ -f "$LAST_REMINDER" ] && reminder_age=$(( NOW - $(date -r "$LAST_REMINDER" +%s 2>/dev/null || echo "$NOW") ))

DIRTY_THRESHOLD=8
AGE_THRESHOLD=1800   # 30 minutes since last checkpoint
COOLDOWN=600         # don't remind more than once per 10 minutes

should_remind=0
if [ "$DIRTY_COUNT" -ge "$DIRTY_THRESHOLD" ]; then
    should_remind=1
elif [ "$checkpoint_age" -ge "$AGE_THRESHOLD" ]; then
    should_remind=1
fi

if [ "$should_remind" -eq 1 ] && [ "$reminder_age" -ge "$COOLDOWN" ]; then
    touch "$LAST_REMINDER"
    mins=$(( checkpoint_age / 60 ))
    echo "{\"systemMessage\": \"context-manager: ${DIRTY_COUNT} file(s) changed, ~${mins} min since last checkpoint — consider running /checkpoint.\"}"
fi

exit 0
