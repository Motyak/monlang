[ "${BASH_SOURCE[0]}" == "$0" ] && { >&2 echo "this script is meant to be sourced, not executed"; exit 1; }
#alias make='TRACE=y BUILD_LIBS_ONCE= make -j16'
alias make='BUILD_LIBS_ONCE= make -j16'
