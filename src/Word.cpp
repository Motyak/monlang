#include <monlang/Word.h>

#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>

#include <utils/vec-utils.h>

MayFail<Word> consumeWord(std::istringstream& input) {
    std::vector<char> terminatorCharacters = vec_union({
        Program::RESERVED_CHARACTERS,
        ProgramSentence::RESERVED_CHARACTERS
    });

    // terminatorCharacters = vec_union({
    //     terminatorCharacters,
    //     ..
    // });

    return mayfail_cast<Word>(consumeAtom(terminatorCharacters, input));
}
