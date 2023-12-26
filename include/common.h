#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <sstream>
#include <functional>

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
/* sentinel values to retrieve certain values at runtime */
const CharacterAppearance TABS = CharacterAppearance{-128, 0};
const CharacterAppearance TABS_PLUS_1 = CharacterAppearance{-128, 1};
const CharacterAppearance TABS_MINUS_1 = CharacterAppearance{-128, 2};

char firstChar(const std::vector<CharacterAppearance>&);
size_t sequenceLen(const std::vector<CharacterAppearance>&);

void consumeSequence(const std::vector<CharacterAppearance>& sequence, std::istringstream&);
bool peekSequence(const std::vector<CharacterAppearance>& sequence, std::istringstream&);

#endif // COMMON_H

