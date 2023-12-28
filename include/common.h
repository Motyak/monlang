#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <sstream>

#define SPACE (char(32))
#define NEWLINE (char(10))
#define TAB (char(9))
#define BACKSLASH (char(92))

extern unsigned g_currentNestedLevel;

struct CharacterAppearance {
    char c;
    unsigned ntimes;
    CharacterAppearance(char c, unsigned ntimes);
    CharacterAppearance(char c);
    operator char() const;
};

char firstChar(const std::vector<CharacterAppearance>&);
size_t sequenceLen(const std::vector<CharacterAppearance>&);

void consumeSequence(const std::vector<CharacterAppearance>& sequence, std::istringstream&);
bool peekSequence(const std::vector<CharacterAppearance>& sequence, std::istringstream&);

#endif // COMMON_H

