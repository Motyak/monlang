#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include <ProgramWord.h>

#include <vector>

struct Association {
    ProgramWordWithoutAssociation leftPart;
    ProgramWord rightPart;

    static const std::vector<char> SEPARATOR_SEQUENCE;

    static const std::vector<std::vector<char>> RESERVED_SEQUENCES;
};

#endif // ASSOCIATION_H
