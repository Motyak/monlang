#include <monlang/Word.h>
#include <monlang/common.h>

/* in impl only */
#include <monlang/ProgramSentence.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/CurlyBracketsGroup.h>
#include <monlang/Atom.h>
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

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
        auto ret = consumeSquareBracketsGroup(input);
        return std::visit(overload{
            [](MayFail<SquareBracketsGroup> sbg){return mayfail_convert<Word>(sbg);},
            [](MayFail<PostfixSquareBracketsGroup> psbg){return mayfail_convert<Word>(psbg);}
        }, ret);
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
            [](MayFail<ParenthesesGroup> pg){return mayfail_convert<Word>(pg);},
            [](MayFail<PostfixParenthesesGroup> ppg){return mayfail_convert<Word>(ppg);},
            [](MayFail<PostfixSquareBracketsGroup> psbg){return mayfail_convert<Word>(psbg);}
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
        [](MayFail<PostfixParenthesesGroup> ppg){return mayfail_convert<Word>(ppg);},
        [](MayFail<PostfixSquareBracketsGroup> psbg){return mayfail_convert<Word>(psbg);}
    }, ret);
}
