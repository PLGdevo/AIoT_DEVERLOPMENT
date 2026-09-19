#!/usr/bin/env bash
# Shared helpers for context-management hooks. Sourced by other scripts in this dir.
# Never touches application source code — reads git state and markdown docs only.

project_root() {
    if [ -n "$CLAUDE_PROJECT_DIR" ]; then
        echo "$CLAUDE_PROJECT_DIR"
    else
        cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd
    fi
}

# extract_section <file> <header>  -- prints lines between "## <header>" and the next "## " line
extract_section() {
    local file="$1"
    local header="$2"
    [ -f "$file" ] || return 0
    awk -v h="## ${header}" '
        $0==h {f=1; next}
        /^## / {f=0}
        f && NF {print}
    ' "$file"
}

git_snapshot() {
    local root
    root="$(project_root)"
    cd "$root" || return 1
    if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
        echo "not a git repo"
        return 0
    fi
    local branch head dirty last_subject
    branch="$(git branch --show-current 2>/dev/null || echo '(detached)')"
    head="$(git rev-parse --short HEAD 2>/dev/null || echo '(no commits)')"
    dirty="$(git status --porcelain 2>/dev/null | wc -l | tr -d ' ')"
    last_subject="$(git log -1 --pretty=%s 2>/dev/null || echo '(none)')"
    printf 'branch=%s head=%s dirty=%s last_commit=%s\n' "$branch" "$head" "$dirty" "$last_subject"
}
