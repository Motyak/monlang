#include <monlang/Word.h>
#include <monlang/common.h>

/* in impl only */
#include <monlang/ProgramSentence.h>
#include <monlang/SquareBracketsTerm.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/CurlyBracketsGroup.h>
#include <monlang/Atom.h>

#include <utils/vec-utils.h>
#include <utils/variant-utils.h>
#include <utils/assert-utils.h>

MayFail<ProgramWord> consumeProgramWord(std::istringstream& input) {

    #ifndef DISABLE_SBT
    if (peekSequence(SquareBracketsTerm::INITIATOR_SEQUENCE, input)) {
        // we use _convert instead of _cast because SBT isn't a composable word
        return mayfail_convert<ProgramWord>(consumeSquareBracketsTerm(input));
    }
    #endif

    return mayfail_cast<ProgramWord>(consumeWord(input));
}

MayFail<Word> consumeWord(std::istringstream& input) {
    std::vector<char> terminatorCharacters;

    terminatorCharacters = vec_union({
        terminatorCharacters,
        ProgramSentence::RESERVED_CHARACTERS
    });

    #ifndef DISABLE_PG
    if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_cast<Word>(consumeParenthesesGroup(input));
    }
    terminatorCharacters = vec_union({
        terminatorCharacters,
        ParenthesesGroup::RESERVED_CHARACTERS
    });
    #endif

    #ifndef DISABLE_SBG
    if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_cast<Word>(consumeSquareBracketsGroup(input));
    }
    terminatorCharacters = vec_union({
        terminatorCharacters,
        SquareBracketsGroup::RESERVED_CHARACTERS
    });
    #endif

    #ifndef DISABLE_CBG
    if (peekSequence(CurlyBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_cast<Word>(consumeCurlyBracketsGroup(input));
    }
    terminatorCharacters = vec_union({
        terminatorCharacters,
        CurlyBracketsGroup::RESERVED_CHARACTERS
    });
    #endif

    /* Atom is the "fall-through" Word */
    return mayfail_cast<Word>(consumeAtom(terminatorCharacters, input));
}

///////////////////////////////////////////////////////////

template <>
MayFail<ProgramWord> mayfail_cast<ProgramWord>(MayFail<Word> inputMayfail) {
    return MayFail((ProgramWord)variant_cast(inputMayfail.val), inputMayfail.err);
}

static Word pw2w(ProgramWord pw) {
    return std::visit(overload{
        [](SquareBracketsTerm*) -> Word {SHOULD_NOT_HAPPEN();},
        [](auto word) -> Word {return word;},
    }, pw);
}

template <>
MayFail<Word> mayfail_cast<Word>(MayFail<ProgramWord> inputMayfail) {
    return MayFail(pw2w(inputMayfail.val), inputMayfail.err);
}
