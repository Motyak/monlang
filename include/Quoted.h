#ifndef QUOTED_H
#define QUOTED_H

#include <common.h>

#include <string>
#include <sstream>
#include <vector>

struct Quoted {
    std::string value;
};

Quoted consumeQuoted(std::istringstream&, const std::vector<CharacterAppearance>& terminatorSequences);

#endif // QUOTED_H
