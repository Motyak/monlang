#include <common.h>
#include <utils/str-utils.h>

#include <iostream>

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

char firstChar(std::vector<CharacterAppearance> sequence) {
    for (auto charAppearance: sequence) {
        if (charAppearance.ntimes > 0) {
            return charAppearance.c;
        }
    }
    return -1;
}

void consumeSequence(std::vector<CharacterAppearance> sequence, std::istringstream& input) {
    for (auto charAppearance: sequence) {
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

bool peekSequence(std::vector<CharacterAppearance> sequence, std::istringstream& input) {
    // save stream position
    std::streampos initialPosition = input.tellg();

    // peek, check, consume      in a loop
    for (auto charAppearance: sequence) {
       if (charAppearance.ntimes == 0) {
            if (input.peek() == charAppearance.c) {
                return false;
            }
            continue;
        }

        for (unsigned i = 1; i <= charAppearance.ntimes; ++i) {
            if (input.peek() != charAppearance.c) {
                return false;
            }
            input.ignore(1);
        }
    }

    // restore stream position
    input.seekg(initialPosition);
    return true;
}
