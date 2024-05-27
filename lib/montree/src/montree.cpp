#include <montree.h>
#include "Print.cpp"

std::string montree::astToString(Ast ast) {
    std::ostringstream oss;
    visitAst(Print(oss), ast);
    return oss.str();
}
