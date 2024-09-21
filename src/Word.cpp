#include <monlang/Word.h>
#include <monlang/common.h>
#include <monlang/Atom.h>

/* in impl only */
#include <monlang/ProgramSentence.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/CurlyBracketsGroup.h>

#include <utils/vec-utils.h>
#include <utils/variant-utils.h>

MayFail<Word> consumeWord(std::istringstream& input) {
    std::vector<char> terminatorCharacters = vec_union({
        ProgramSentence::RESERVED_CHARACTERS
    });

    // terminatorCharacters = vec_union({
    //     terminatorCharacters,
    //     ..
    // });

#ifndef DISABLE_SBG
    terminatorCharacters = vec_union({
        terminatorCharacters,
        SquareBracketsGroup::RESERVED_CHARACTERS
    });
    if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_convert<Word>(consumeSquareBracketsGroup(input));
    }
#endif

#ifndef DISABLE_PG
    terminatorCharacters = vec_union({
        terminatorCharacters,
        ParenthesesGroup::RESERVED_CHARACTERS
    });
    if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        auto ret = consumeParenthesesGroup(input);
        return std::visit(overload{
            [](MayFail<ParenthesesGroup> atom){return mayfail_convert<Word>(atom);},
            [](MayFail<PostfixParenthesesGroup> ppg){return mayfail_convert<Word>(ppg);}
        }, ret);
    }
#endif

#ifndef DISABLE_CBG
    terminatorCharacters = vec_union({
        terminatorCharacters,
        CurlyBracketsGroup::RESERVED_CHARACTERS
    });
    if (peekSequence(CurlyBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_convert<Word>(consumeCurlyBracketsGroup(input));
    }
#endif

    /* Atom is the "fall-through" Word */

    auto ret = consumeAtom(terminatorCharacters, input);
    return std::visit(overload{
        [](MayFail<Atom> atom){return mayfail_convert<Word>(atom);},
        [](MayFail<PostfixParenthesesGroup> ppg){return mayfail_convert<Word>(ppg);}
    }, ret);
}
