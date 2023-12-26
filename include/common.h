#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <sstream>

#define SPACE (char(32))
#define NEWLINE (char(10))
#define TAB (char(9))
#define BACKSLASH (char(92))

#define TABS CharacterAppearance{TAB, currentNestedLevel()}
#define TABS_PLUS_1 CharacterAppearance{TAB, currentNestedLevel() + 1}

extern unsigned g_currentNestedLevel;
unsigned currentNestedLevel();

struct CharacterAppearance {
    char c;
    unsigned ntimes;
    CharacterAppearance(char c, unsigned ntimes);
    CharacterAppearance(char c);
    operator char() const;
};

char firstChar(std::vector<CharacterAppearance>);
size_t sequenceLen(std::vector<CharacterAppearance>);

void consumeSequence(std::vector<CharacterAppearance> sequence, std::istringstream&);
bool peekSequence(std::vector<CharacterAppearance> sequence, std::istringstream&);

#endif // COMMON_H

