#!/bin/bash

# assumed unchanged by git
INPUT="
include/utils
utils.mk
"

set -o errexit
set -o pipefail

files="$(echo "$INPUT" | xargs)"
for file in $files; do
    [ -L "$file" ] || continue # is a symlink (broken or not)
    [ -f "$file" ] || [ -d "$file" ] || continue # links to a regular file or directory
    target_file="$(readlink -f "$file")"
    commands=$(cat << EOF
mv "$file" "${file}.bak"
cp -r "$target_file" "$file"
git add "$file"
rm -rf "$file"
mv "${file}.bak" "$file"
EOF
    )
    >&2 echo "about to run the following commands: "
    >&2 awk '{print "\t" $0}' <<< "$commands"
    >&2 echo -n "confirm?(Y/n) >"
    read confirm
    [[ "$confirm" =~ n|N ]] && { >&2 echo "aborted"; exit; }
    bash -x -c "$commands"
    >&2 echo
done
