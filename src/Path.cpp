#include <monlang/Path.h>
#include <monlang/Word.h>

/* require knowing all words for token_len() */
#include <monlang/ast/token_len.h>

#include <utils/mem-utils.h>

const Sequence Path::SEPARATOR_SEQUENCE = {'.'};

const std::vector<char> Path::RESERVED_CHARACTERS = {
    sequenceFirstChar(Path::SEPARATOR_SEQUENCE).value()
};

Malformed<Malformed_<Path>>
consumePath(PostfixLeftPart& accumulatedPostfixLeftPart, const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    input.ignore(1); // .
    auto whats_right_behind = consumeAtom(terminatorCharacters, input);

    if (whats_right_behind.has_error()) {
        auto path = MayFail_<Path>{
            accumulatedPostfixLeftPart,
            whats_right_behind
        };
        return Malformed(path, ERR(229));
    }

    auto path = Path{
        accumulatedPostfixLeftPart,
        (Atom)whats_right_behind
    };
    path._tokenLen = token_len(accumulatedPostfixLeftPart) + GET_INPUT_STREAM_PROGRESS();
    accumulatedPostfixLeftPart = move_to_heap(path);

    // return value is only used in presence of error,..
    // ..in the absence of error what matters is the assignment of the OUT parameter..
    // ..therefore the return value here is useless

    return wrap(path);
}

///////////////////////////////////////////////////////////

Path::Path(const PathLeftPart& leftPart, const Atom& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<Path>::MayFail_(const PathLeftPart& leftPart, const MayFail<Atom>& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<Path>::MayFail_(const Path& path) {
    this->leftPart = path.leftPart;
    this->rightPart = path.rightPart;
    this->_tokenLen = path._tokenLen;
    this->_tokenId = path._tokenId;
}

MayFail_<Path>::operator Path() const {
    auto path = Path{leftPart, (Atom)rightPart};
    path._tokenLen = this->_tokenLen;
    path._tokenId = this->_tokenId;
    return path;
}

