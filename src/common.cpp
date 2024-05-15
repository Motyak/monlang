#include <common.h>

Error::operator unsigned() const {
    return this->code;
}

CharacterAppearance::CharacterAppearance(char c, unsigned ntimes) : c(c), ntimes(ntimes) {
    ;
}

CharacterAppearance::CharacterAppearance(char c) : CharacterAppearance{c, 1} {
    ;
}

CharacterAppearance::operator char() const {
    return this->c;
}

std::optional<char> sequenceFirstChar(const std::vector<CharacterAppearance>& sequence) {
    for (auto charAppearance: sequence) {
        if (charAppearance.ntimes > 0) {
            return charAppearance.c;
        }
    }
    return {};
}

size_t sequenceLen(const std::vector<CharacterAppearance>& sequence) {
    size_t len = 0;
    for (auto charAppearance: sequence) {
        len += charAppearance.ntimes;
    }
    return len;
}

std::expected<void, Error> consumeSequence(const std::vector<CharacterAppearance>& sequence, std::istringstream& input) {
    for (auto charAppearance: sequence) {
        if (charAppearance.ntimes == 0) {
            if (input.peek() == charAppearance.c) {
                return std::unexpected(Error{100});
            }
            continue;
        }

        for (unsigned i = 1; i <= charAppearance.ntimes; ++i) {
            if (input.peek() != charAppearance.c) {
                return std::unexpected(Error{101});
            }
            input.ignore(1);
        }
    }

    ok();
}

bool peekSequence(const std::vector<CharacterAppearance>& sequence, std::istringstream& input) {
    // save stream position
    std::streampos initialPosition = input.tellg();

    for (auto charAppearance: sequence) {
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
