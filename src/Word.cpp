#include <monlang/Word.h>
#include <monlang/common.h>
#include <monlang/Atom.h>

/* in impl only */
#include <monlang/ProgramSentence.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/ParenthesesGroup.h>

#include <utils/vec-utils.h>

MayFail<Word> consumeWord(std::istringstream& input) {
    std::vector<char> terminatorCharacters = vec_union({
        ProgramSentence::RESERVED_CHARACTERS
    });

    // terminatorCharacters = vec_union({
    //     terminatorCharacters,
    //     ..
    // });

#ifndef DISABLE_SBG
    if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_convert<Word>(consumeSquareBracketsGroup(input));
    }
    terminatorCharacters = vec_union({
        terminatorCharacters,
        SquareBracketsGroup::RESERVED_CHARACTERS
    });
#endif

#ifndef DISABLE_PG
    if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_convert<Word>(consumeParenthesesGroup(input));
    }
    terminatorCharacters = vec_union({
        terminatorCharacters,
        ParenthesesGroup::RESERVED_CHARACTERS
    });
#endif

    // Atom is the "fall-through" Word
    return mayfail_cast<Word>(consumeAtom(terminatorCharacters, input));
}
