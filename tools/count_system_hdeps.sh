#!/bin/bash
shopt -s globstar nullglob

function print_direct_system_dep {
    local file="$1"

    # shared between `_print_direct_system_dep` function calls
    g_blacklist="a" # stub for regex cmp

    _print_direct_system_dep "$1" \
    | sort -u
}

# recursive function
function _print_direct_system_dep {
    local file="$1"
    local direct_deps
    local direct_system_deps
    local direct_user_deps

    direct_deps="$(cpp <(cat "$file") -M -MG -MF - \
        | xargs | xargs | cut -d ' ' -f 4- | tr ' ' '\n'
    )"
    direct_system_deps="$(cat <<< " $direct_deps" \
        | grep -v '.h$'
    )"
    direct_user_deps="$(cat <<< " $direct_deps" \
        | grep '.h$'
    )"

    ## handle direct SYSTEM dependencies ##
    for d in $direct_system_deps; do
        echo "$d"
        >&2 echo "$file $d OK"
    done

    ## handle direct USER dependencies ##
    for d in $direct_user_deps; do
        [[ " $g_blacklist" =~ \ $d ]] && continue
        g_blacklist="$g_blacklist $d"
        _print_direct_system_dep "${INCLUDE_DIR%/}/${d}"
    done
}

# set -o xtrace #debug

SRC_DIR="${1:-.}"; >&2 echo "\$1 SRC_DIR=\`${SRC_DIR}\`"
INCLUDE_DIR="${2:-${SRC_DIR}}"; >&2 echo "\$2 INCLUDE_DIR=\`${INCLUDE_DIR}\`"
>&2 echo

src_files=("${SRC_DIR%/}"**/*.cpp)
>&2 echo "${#src_files[@]} src file(s) found"
>&2 echo

{
    for src in "${src_files[@]}"; do
        >&2 echo "Processing ${src}..."
        print_direct_system_dep "$src"
        >&2 echo -e "$src OK\n"
    done
} \
| sort \
| uniq -c \
| sort -k1nr -k2 \
| perl -pe 's/(\d+)/$1\/'"${#src_files[@]}"'/g'

# # ask preprocessor for system header search paths
# search_paths="$(cpp -v /dev/null -o /dev/null 2>&1 \
#     | perl -e 'my $s = do { local $/; <> }; $s =~ /#include <.*\n((?: .*\n)+)/ && print $1' \
#     | xargs)"
