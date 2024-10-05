[ "${BASH_SOURCE[0]}" == "$0" ] && { >&2 echo "this script is meant to be sourced, not executed"; exit 1; }

alias make='make -j16 BUILD_LIBS_ONCE='
#alias make='make -j16 TRACE=y BUILD_LIBS_ONCE='

## custom build
#alias make='make -j16 BUILD_LIBS_ONCE= DISABLE_WORDS=SBG DISABLE_POSTFIXES=PG_IN_PG'
