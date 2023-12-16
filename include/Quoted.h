#ifndef QUOTED_H
#define QUOTED_H

#include <string>
#include <sstream>
#include <vector>

struct Quoted {
    std::string value;
};

Quoted consumeQuoted(std::istringstream&, const std::vector<char> terminatorSequence);

#endif // QUOTED_H
