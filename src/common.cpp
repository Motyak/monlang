#include <common.h>
#include <utils/str-utils.h>

#include <iostream>

unsigned g_currentNestedLevel = 0;

/* sentinel values to retrieve certain values at runtime */
const CharacterAppearance TABS = CharacterAppearance{-128, 0};
const CharacterAppearance TABS_PLUS_1 = CharacterAppearance{-128, 1};
const CharacterAppearance TABS_MINUS_1 = CharacterAppearance{-128, 2};

CharacterAppearance::CharacterAppearance(char c, unsigned ntimes) {
    this->c = c;
    this->ntimes = ntimes;
}

CharacterAppearance::CharacterAppearance(char c) : CharacterAppearance{c, 1} {
    ;
}

CharacterAppearance::operator char() const {
    return this->c;
}

char firstChar(const std::vector<CharacterAppearance>& sequence) {
    for (auto charAppearance: sequence) {
        if (charAppearance.ntimes > 0) {
            return charAppearance.c;
        }
    }
    return -1;
}

size_t sequenceLen(const std::vector<CharacterAppearance>& sequence) {
    size_t len = 0;
    for (auto charAppearance: sequence) {
        len += charAppearance.ntimes;
    }
    return len;
}

// for values that needs to be retrieved at runtime
static void evalSpecialCharAppearance(CharacterAppearance&);

void consumeSequence(const std::vector<CharacterAppearance>& sequence, std::istringstream& input) {
    for (auto charAppearance: sequence) {
        evalSpecialCharAppearance(charAppearance);

        if (charAppearance.ntimes == 0) {
            if (input.peek() == charAppearance.c) {
                std::cerr << "was expecting ZERO times character "
                        << str(charAppearance.c) << std::endl;
                throw std::runtime_error("user exception");
            }
            continue;
        }

        for (unsigned i = 1; i <= charAppearance.ntimes; ++i) {
            if (input.peek() != charAppearance.c) {
                    std::cerr << "was expecting " << str(charAppearance.c) 
                            << " but found " << str(input.peek()) << std::endl;
                throw std::runtime_error("user exception");
            }
            input.ignore(1);
        }
    }
}

bool peekSequence(const std::vector<CharacterAppearance>& sequence, std::istringstream& input) {
    // save stream position
    std::streampos initialPosition = input.tellg();

    for (auto charAppearance: sequence) {
        evalSpecialCharAppearance(charAppearance);

        if (charAppearance.ntimes == 0) {
            if (input.peek() == charAppearance.c) {
                // restore stream position
                input.seekg(initialPosition);
                return false;
            }
            continue;
        }

        for (unsigned i = 1; i <= charAppearance.ntimes; ++i) {
            if (input.peek() != charAppearance.c) {
                // restore stream position
                input.seekg(initialPosition);
                return false;
            }
            input.ignore(1);
        }
    }

    // restore stream position
    input.seekg(initialPosition);
    return true;
}

static void evalSpecialCharAppearance(CharacterAppearance& charAppearance) {
    if (charAppearance == TABS) {
        charAppearance.c = TAB;
        charAppearance.ntimes = g_currentNestedLevel;
    }
    else if (charAppearance == TABS_PLUS_1) {
        charAppearance.c = TAB;
        charAppearance.ntimes = g_currentNestedLevel + 1;
    }
    else if (charAppearance == TABS_MINUS_1) {
        charAppearance.c = TAB;
        charAppearance.ntimes = g_currentNestedLevel - 1;
    }
}
