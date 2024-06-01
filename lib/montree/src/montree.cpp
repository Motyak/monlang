#include "Print.cpp"

#include <montree.h>
#include <monlang/visitors/visitor.h>

std::string montree::astToString(Ast ast) {
    std::ostringstream oss;
    visitAst(Print(oss), ast);
    auto res = oss.str();
    // remove potential trailing newline
    if (!res.empty() && res.back() == NEWLINE) {
        res.pop_back();
    }
    return res;
}
