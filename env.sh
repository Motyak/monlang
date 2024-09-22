[ "${BASH_SOURCE[0]}" == "$0" ] && { >&2 echo "this script is meant to be sourced, not executed"; exit 1; }

## recommended
alias make='BUILD_LIBS_ONCE= make -j16'

## debug
#alias make='TRACE=y BUILD_LIBS_ONCE= make'

## custom build
#alias make='BUILD_LIBS_ONCE= make -j16 DISABLE_WORDS=SBG DISABLE_POSTFIXES=PG_IN_PG'
