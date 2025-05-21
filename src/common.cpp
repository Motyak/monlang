#include <monlang/common.h>

#include <utils/assert-utils.h>
#include <utils/str-utils.h>

#include <limits>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Error& err) {
    return os << err.fmt;
}

////////////////////////////////////////////////////////////////

Quantifier::Quantifier(int n) : n(n) {
    ;
}

Quantifier::operator int() const {
    return n;
}

Quantifier operator ""_(unsigned long long int n) {
    ASSERT(n <= std::numeric_limits<int>::max());
    return Quantifier{int(n)};
}

CharacterAppearance::CharacterAppearance(char c, Quantifier ntimes) : c(c), ntimes(ntimes) {
    ;
}

Sequence operator *(int ntimes, const Sequence& seq) {
    ASSERT(ntimes >= 0);
    Sequence res;
    for (int i = 1; i <= ntimes; ++i) {
        for (size_t j = 0; j < seq.size(); ++j) {
            res.push_back(seq[j]);
        }
    }
    return res;
}

std::ostream& operator<<(std::ostream& out, const Sequence& seq) {
    for (auto charAppearance: seq) {
        for (int i = 1; i <= charAppearance.ntimes; ++i) {
            out << charAppearance.c;
        }
    }
    return out;
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

MayFail<void> consumeSequence(const std::vector<CharacterAppearance>& sequence, std::istringstream& input) {
    for (auto charAppearance: sequence) {
        if (charAppearance.ntimes == 0) {
            if (input.peek() == charAppearance.c) {
                return ERR(100);
            }
            continue;
        }

        for (int i = 1; i <= charAppearance.ntimes; ++i) {
            if (input.peek() != charAppearance.c) {
                return ERR(101);
            }
            input.ignore(1);
        }
    }

    return OK();
}

bool peekSequence(const std::vector<CharacterAppearance>& sequence, std::istringstream& input) {
    // save stream position
    std::streampos initialPosition = input.tellg();

    for (auto charAppearance: sequence) {
        if (charAppearance.ntimes == 0) {
            if (input.peek() == charAppearance.c) {
                // reset flags if EOF reached etc..
                input.clear();
                // restore stream position
                input.seekg(initialPosition);
                return false;
            }
            continue;
        }

        for (int i = 1; i <= charAppearance.ntimes; ++i) {
            if (input.peek() != charAppearance.c) {
                // reset flags if EOF reached etc..
                input.clear();
                // restore stream position
                input.seekg(initialPosition);
                return false;
            }
            input.ignore(1);
        }
    }

    // reset flags if EOF reached etc..
    input.clear();
    // restore stream position
    input.seekg(initialPosition);
    return true;
}

bool peekStrUntil(const std::string& str, const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    // save stream position
    std::streampos initialPosition = input.tellg();

    for (auto c: str) {
        if (input.peek() != c) {
            // reset flags if EOF reached etc..
            input.clear();
            // restore stream position
            input.seekg(initialPosition);
            return false;
        }
        input.ignore(1);
    }

    for (auto c: terminatorCharacters) {
        if (input.peek() == c) {
            // reset flags if EOF reached etc..
            input.clear();
            // restore stream position
            input.seekg(initialPosition);
            return true;
        }
    }

    // reset flags if EOF reached etc..
    input.clear();
    // restore stream position
    input.seekg(initialPosition);
    return false;
}

bool peekAnyChar(const std::vector<char>& chars, std::istringstream& input) {
    for (auto c: chars) {
        if (input.peek() == c) {
            return true;
        }
    }
    return false;
}

bool peekAnySeq(const std::vector<Sequence>& seqs, std::istringstream& input) {
    for (auto seq: seqs) {
        if (peekSequence(seq, input)) {
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////

thread_local int _TRACE_CUR_FUNC::depth = 0;

_TRACE_CUR_FUNC::_TRACE_CUR_FUNC(const std::string& funcName, std::istringstream& input) : funcName(funcName), input(input) {
#ifdef TRACE
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
#endif // TRACE
}

_TRACE_CUR_FUNC::~_TRACE_CUR_FUNC() {
#ifdef TRACE
    depth--;
    for (int i = 1; i <= depth; ++i) {
        std::cerr << "│   ";
    }

    std::cerr << funcName << " END";

    std::string remaining = input.tellg() == -1? "" : input.str().substr(input.tellg());
    auto remainingAsLines = split(remaining, "\n");
    ASSERT(remainingAsLines.size() > 0);
    std::cerr << " `" << remainingAsLines[0] << "`";
    if (remainingAsLines.size() > 1) {
        std::cerr << " (" << remainingAsLines.size() - 1 << " more ";
        std::cerr << (remainingAsLines.size() > 2? "lines" : "line") << ")";
    }

    std::cerr << std::endl;
#endif // TRACE
}
