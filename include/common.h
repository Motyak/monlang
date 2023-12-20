#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <sstream>

#define SPACE (char(32))
#define NEWLINE (char(10))

void consumeSequence(std::vector<char> sequence, std::istringstream&);
bool peekSequence(std::vector<char> sequence, std::istringstream&);

#endif // COMMON_H

