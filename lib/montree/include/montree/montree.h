#ifndef MONTREE_H
#define MONTREE_H

#include <monlang/visitors/visitor.h>

namespace montree {
    std::string astToString(Ast_);
    std::string astToString(Ast_, int TAB_SIZE);
}

#endif // MONTREE_H
