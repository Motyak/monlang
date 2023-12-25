#ifndef QUOTED_H
#define QUOTED_H

#include <common.h>

#include <string>
#include <sstream>
#include <vector>

struct Quoted {
    std::string value;
};

Quoted consumeOnelineQuoted(std::istringstream&);
Quoted consumeMultilineQuoted(std::istringstream&);

#endif // QUOTED_H
