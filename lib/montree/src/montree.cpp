#include "Print.cpp"

#include <montree/montree.h>

#include <monlang/visitors/visitor.h>

#include <iostream>

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

std::string montree::astToString(Ast ast, int TAB_SIZE) {
    std::ostringstream oss;
    visitAst(Print(oss, TAB_SIZE), ast);
    auto res = oss.str();
    // remove potential trailing newline
    if (!res.empty() && res.back() == NEWLINE) {
        res.pop_back();
    }
    return res;
}
