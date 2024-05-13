#ifndef ATOM_H
#define ATOM_H

#include <variant>
#include <sstream>

struct Atom {
    std::string value;
};

Atom consumeAtom(const std::vector<char>& terminatorCharacters, std::istringstream& input);

#endif // ATOM_H
