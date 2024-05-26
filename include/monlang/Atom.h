#ifndef ATOM_H
#define ATOM_H

#include <monlang/common.h>

struct Atom {
    std::string value;
};

MayFail<Atom> consumeAtom(const std::vector<char>& terminatorCharacters, std::istringstream& input);

#endif // ATOM_H
