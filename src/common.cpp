#include <monlang/common.h>

#include <utils/str-utils.h>

thread_local int _TRACE_CUR_FUNC::depth = 0;

_TRACE_CUR_FUNC::_TRACE_CUR_FUNC(std::string funcName, std::istringstream& input) : funcName(funcName), input(input) {
    for (int i = 1; i <= depth - 1; ++i) {
        std::cerr << "│   ";
    }
    if (depth >= 1) {
        std::cerr << "├───";
    }

    std::cerr << funcName << " BEGIN";

    std::string remaining = input.str().substr(input.tellg());
    auto remainingAsLines = split(remaining, "\n");
    ASSERT(remainingAsLines.size() > 0);
    std::cerr << " `" << remainingAsLines[0] << "`";
    if (remainingAsLines.size() > 1) {
        std::cerr << " (" << remainingAsLines.size() - 1 << " more ";
        std::cerr << (remainingAsLines.size() > 2? "lines" : "line") << ")";
    }

    std::cerr << std::endl;
    depth++;
}

_TRACE_CUR_FUNC::~_TRACE_CUR_FUNC() {
    depth--;
    for (int i = 1; i <= depth; ++i) {
        std::cerr << "│   ";
    }

    std::cerr << funcName << " END";

    std::string remaining = input.str().substr(input.tellg());
    auto remainingAsLines = split(remaining, "\n");
    ASSERT(remainingAsLines.size() > 0);
    std::cerr << " `" << remainingAsLines[0] << "`";
    if (remainingAsLines.size() > 1) {
        std::cerr << " (" << remainingAsLines.size() - 1 << " more ";
        std::cerr << (remainingAsLines.size() > 2? "lines" : "line") << ")";
    }

    std::cerr << std::endl;
}

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

    OK();
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
