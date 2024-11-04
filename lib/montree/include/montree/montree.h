#ifndef MONTREE_H
#define MONTREE_H

#include <monlang/visitors/visitor.h>

namespace montree {
    std::string astToString(Ast);
    std::string astToString(Ast, int TAB_SIZE);
}

#endif // MONTREE_H
