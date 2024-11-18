#!/bin/bash

# pass REVERT=x as variable to revert the script

INPUT="
include/utils
utils.mk
lib/montree
"

set -o errexit
set -o pipefail

set -o xtrace #debug

# add 'ignored' alias if missing
>/dev/null git config --global --get-regexp '^alias\.ignored' \
    || git config --global alias.ignored '!git ls-files -v | grep "^[[:lower:]]" || true'

git ls-files -z $INPUT | xargs -0 git update-index --${REVERT:+no-}assume-unchanged
