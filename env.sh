[ "${BASH_SOURCE[0]}" == "$0" ] && { >&2 echo "this script is meant to be sourced, not executed"; exit 1; }

#alias make='make -j16 BUILD_LIBS_ONCE='
#alias make='make -j16 TRACE=y BUILD_LIBS_ONCE='

## custom build
#alias make='make -j16 BUILD_LIBS_ONCE= DISABLE_WORDS=SBG DISABLE_POSTFIXES=PG_IN_PG'

## uncomment below line if you intend to use an above alias instead of the `make` function
#return 0
function make {
    local MAKE="/usr/bin/make"
    local EXTRA_ARGS="-j16 BUILD_LIBS_ONCE="

    local opt_args=""
    local target_args=""
    for arg in "$@"; do
        case $arg in
        -* | *=*)
            opt_args="${opt_args}${opt_args:+ }${arg}"
            ;;
        *)
            target_args="${target_args}${target_args:+ }${arg}"
            ;;
        esac
    done

    local make_prefix="$MAKE $EXTRA_ARGS $opt_args"
    local final_cmd=""
    for target in $target_args; do
        final_cmd="${final_cmd}${final_cmd:+ && }${make_prefix} $target"
    done

    echo "DEBUG opt args: \`$opt_args\`"
    echo "DEBUG target args: \`$target_args\`"

    case "$make_prefix" in
    *\ -q* | *\ --question*)
        # dry run first
        echo eval "$make_prefix $target_args"
        echo exit $?
        ;;
    esac

    # dry run first
    echo eval "${final_cmd:-$MAKE}"
}

## add (back) make autocompletion for our new definition of make
source tools/bash_completion_make # exports _make function
complete -F _make make
