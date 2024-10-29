#define POSTFIX_SQUARE_BRACKETS_GROUP_CPP

#include <monlang/PostfixSquareBracketsGroup.h>

#if __has_include ("sequence.hpp")
    // enable extern explicit instanciations..
    // ..for common.h 'sequence' templates
    #include "sequence.hpp"
    #include "sequence.tpp"
#endif

#if __has_include ("mayfail.hpp")
    // enable extern explicit instanciations..
    // ..for common.h 'mayfail' templates
    #include "mayfail.hpp"
    #include "mayfail.tpp"
#endif

// dummy cpp file in order for it to get its own object file
