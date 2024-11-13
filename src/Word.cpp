#include <monlang/Word.h>
#include <monlang/common.h>

/* in impl only */

#include <monlang/ProgramSentence.h>
#include <monlang/SquareBracketsTerm.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/CurlyBracketsGroup.h>
#include <monlang/Atom.h>

/* required by the (un)wrap functions only */
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/Association.h>

#include <utils/vec-utils.h>
#include <utils/variant-utils.h>
#include <utils/assert-utils.h>

MayFail<ProgramWord_> consumeProgramWord(std::istringstream& input) {

    #ifndef DISABLE_SBT
    if (peekSequence(SquareBracketsTerm::INITIATOR_SEQUENCE, input)) {
        // we use _convert instead of _cast because SBT isn't a composable word
        return mayfail_convert<ProgramWord_>(consumeSquareBracketsTerm(input));
    }
    #endif

    return mayfail_cast<ProgramWord_>(consumeWord(input));
}

MayFail<Word_> consumeWord(std::istringstream& input) {
    std::vector<char> terminatorCharacters;

    terminatorCharacters = vec_union({
        terminatorCharacters,
        ProgramSentence::RESERVED_CHARACTERS,
        Term::RESERVED_CHARACTERS,
    });

    #ifndef DISABLE_PG
    if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_cast<Word_>(consumeParenthesesGroup(input));
    }
    terminatorCharacters = vec_union({
        terminatorCharacters,
        ParenthesesGroup::RESERVED_CHARACTERS
    });
    #endif

    #ifndef DISABLE_SBG
    if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_cast<Word_>(consumeSquareBracketsGroup(input));
    }
    terminatorCharacters = vec_union({
        terminatorCharacters,
        SquareBracketsGroup::RESERVED_CHARACTERS
    });
    #endif

    #ifndef DISABLE_CBG
    if (peekSequence(CurlyBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_cast<Word_>(consumeCurlyBracketsGroup(input));
    }
    terminatorCharacters = vec_union({
        terminatorCharacters,
        CurlyBracketsGroup::RESERVED_CHARACTERS
    });
    #endif

    /* Atom is the "fall-through" Word */
    return mayfail_cast<Word_>(consumeAtom(terminatorCharacters, input));
}

///////////////////////////////////////////////////////////

/*
    for the (un)wrappers to work we not only need..
    ..Postfixes/Assoc interfaces, but their impl as well
*/

ProgramWord unwrap_pw(ProgramWord_ pw_) {
    return std::visit(overload{
        [](Atom* atom) -> ProgramWord {return atom;},
        [](auto pw_) -> ProgramWord {return move_to_heap(pw_->unwrap());}
    }, pw_);
}

Word unwrap_w(Word_ word_) {
    return std::visit(overload{
        [](Atom* atom) -> Word {return atom;},
        [](auto word_) -> Word {return move_to_heap(word_->unwrap());}
    }, word_);
}

ProgramWord_ wrap_pw(ProgramWord pw) {
    return std::visit(overload{
        [](Atom* atom) -> ProgramWord_ {return atom;},
        [](auto pw) -> ProgramWord_ {return move_to_heap(pw->wrap());}
    }, pw);
}

Word_ wrap_w(Word word) {
    return std::visit(overload{
        [](Atom* atom) -> Word_ {return atom;},
        [](auto word) -> Word_ {return move_to_heap(word->wrap());}
    }, word);
}

template <>
MayFail<ProgramWord_> mayfail_cast<ProgramWord_>(MayFail<Word_> inputMayfail) {
    return MayFail((ProgramWord_)variant_cast(inputMayfail.val), inputMayfail.err);
}

static Word_ pw2w(ProgramWord_ pw) {
    return std::visit(overload{
        [](MayFail_<SquareBracketsTerm>*) -> Word_ {SHOULD_NOT_HAPPEN();},
        [](auto word) -> Word_ {return word;},
    }, pw);
}

template <>
MayFail<Word_> mayfail_cast<Word_>(MayFail<ProgramWord_> inputMayfail) {
    return MayFail(pw2w(inputMayfail.val), inputMayfail.err);
}
