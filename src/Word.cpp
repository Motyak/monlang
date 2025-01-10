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

/* required for casting Word to Term (while preserving _tokenLen) */
#include <monlang/Term.h>

#include <utils/vec-utils.h>
#include <utils/variant-utils.h>
#include <utils/assert-utils.h>

static Atom SpecialAtom(const std::string& value) {
    auto atom = Atom{value};
    atom._tokenLen = value.size();
    return atom;
}

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
    std::vector<Atom> specialAtoms;

    terminatorCharacters = vec_union({
        terminatorCharacters,
        ProgramSentence::RESERVED_CHARACTERS,
        Term::RESERVED_CHARACTERS,
    });

    #ifndef DISABLE_SPECIAL_ATOMS
    specialAtoms = {
        SpecialAtom(":="), // assignment symbol
        SpecialAtom("[:]"), // map literal
        SpecialAtom(".."), // range operator
    };
    #endif
    for (auto atom: specialAtoms) {
        if (peekStrUntil(atom.value, terminatorCharacters, input)) {
            input.ignore(atom.value.size());
            return (Word_)move_to_heap(atom);
        }
    }

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

bool holds_word(const ProgramWord& pw) {
    return std::visit(overload{
        [](SquareBracketsTerm*) {return false;},
        [](auto) {return true;},
    }, pw);
}

Word get_word(const ProgramWord& pw) {
    return std::visit(overload{
        [](SquareBracketsTerm*) -> Word {SHOULD_NOT_HAPPEN();},
        [](auto word) -> Word {return word;},
    }, pw);
}

Term as_term(const Word& word) {
    auto words = std::vector<Word>{word};
    auto term = Term{words};
    term._tokenLen = token_len(word); // require knowing..
                                      // ..about all words
    return term;
}

Word as_word(const Term& term) {
    auto word = term.words.at(0);
    set_token_len(word, term._tokenLen); // requires knowing..
                                         // ..about all words
    return word;
}

/*
    for the (un)wrappers to work we not only need..
    ..Postfixes/Assoc interfaces, but their impl as well
*/

ProgramWord unwrap_pw(const ProgramWord_& pw_) {
    return std::visit(overload{
        [](Atom* atom) -> ProgramWord {return atom;},
        [](auto pw_) -> ProgramWord {return move_to_heap(unwrap(*pw_));}
    }, pw_);
}

Word unwrap_w(const Word_& word_) {
    return std::visit(overload{
        [](Atom* atom) -> Word {return atom;},
        [](auto word_) -> Word {return move_to_heap(unwrap(*word_));}
    }, word_);
}

ProgramWord_ wrap_pw(const ProgramWord& pw) {
    return std::visit(overload{
        [](Atom* atom) -> ProgramWord_ {return atom;},
        [](auto pw) -> ProgramWord_ {return move_to_heap(wrap(*pw));}
    }, pw);
}

Word_ wrap_w(const Word& word) {
    return std::visit(overload{
        [](Atom* atom) -> Word_ {return atom;},
        [](auto word) -> Word_ {return move_to_heap(wrap(*word));}
    }, word);
}

template <>
MayFail<ProgramWord_> mayfail_cast<ProgramWord_>(const MayFail<Word_>& inputMayfail) {
    return MayFail((ProgramWord_)variant_cast(inputMayfail.val), inputMayfail.err);
}

static Word_ pw2w(const ProgramWord_& pw) {
    return std::visit(overload{
        [](MayFail_<SquareBracketsTerm>*) -> Word_ {SHOULD_NOT_HAPPEN();},
        [](auto word) -> Word_ {return word;},
    }, pw);
}

template <>
MayFail<Word_> mayfail_cast<Word_>(const MayFail<ProgramWord_>& inputMayfail) {
    return MayFail(pw2w(inputMayfail.val), inputMayfail.err);
}
