#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <sstream>

#define SPACE (char(32))
#define NEWLINE (char(10))
#define BACKSLASH (char(92))

struct CharacterAppearance {
    char c;
    unsigned ntimes;
    CharacterAppearance(char c, unsigned ntimes);
    CharacterAppearance(char c);
    operator char() const;
};

void consumeSequence(std::vector<CharacterAppearance> sequence, std::istringstream&);
bool peekSequence(std::vector<CharacterAppearance> sequence, std::istringstream&);

#endif // COMMON_H

