#!/usr/bin/env bash
# PreCompact hook: captures a cheap git-state snapshot to local cache right
# before context gets compacted, so a later /checkpoint or /end-session can
# diff "what changed since the last compaction" without needing the
# (about to be summarized) conversation history. Never touches app source.
set -u
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$DIR/lib.sh"

cat >/dev/null 2>&1 || true

ROOT="$(project_root)"
CACHE_DIR="$ROOT/.claude/tools/context/.cache"
mkdir -p "$CACHE_DIR"

cd "$ROOT" || exit 0
STAMP="$(date -u +%Y%m%dT%H%M%SZ)"
SNAPSHOT="$CACHE_DIR/pre-compact-${STAMP}.txt"

{
    echo "timestamp_utc=${STAMP}"
    git_snapshot
    echo "--- git status --porcelain ---"
    git status --porcelain 2>/dev/null
    echo "--- git diff --stat ---"
    git diff --stat 2>/dev/null
} > "$SNAPSHOT" 2>/dev/null

# Keep only the 5 most recent snapshots.
ls -1t "$CACHE_DIR"/pre-compact-*.txt 2>/dev/null | tail -n +6 | xargs -r rm -f

echo '{"systemMessage": "context-manager: pre-compact git snapshot saved."}'
