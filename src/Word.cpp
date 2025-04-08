#include <monlang/Word.h>
#include <monlang/common.h>

/* in impl only */

#include <monlang/ProgramSentence.h>
#include <monlang/SquareBracketsTerm.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/CurlyBracketsGroup.h>
#include <monlang/Atom.h>
#include <monlang/Quotation.h>
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/Path.h>
#include <monlang/Association.h>

/* required for casting Word to Term (while preserving _tokenLen) */
#include <monlang/Term.h>

#include <utils/vec-utils.h>
#include <utils/variant-utils.h>
#include <utils/assert-utils.h>

namespace {
    using WordStrictly_ = std::variant<
        Atom*,
        Quotation*,
        /* no MayFail_<SquareBracketsTerm>* here */
        GROUP_ENTITIES_
        /* no POSTFIXES_ here */
        /* no MayFail_<Path>* here */
        /* no MayFail_<Association>* here */
    >;
}

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

static MayFail<WordStrictly_> consumeWordStrictly(std::vector<char>& terminatorCharacters, std::istringstream& input) {
    std::vector<Atom> specialAtoms;

    terminatorCharacters = {EOF};
    terminatorCharacters = vec_union({
        terminatorCharacters,
        ProgramSentence::RESERVED_CHARACTERS,
        Term::RESERVED_CHARACTERS,
        #if !defined DISABLE_PG || !defined DISABLE_PPG
        ParenthesesGroup::RESERVED_CHARACTERS,
        #endif
        #if !defined DISABLE_SBG || !defined DISABLE_PSBG
        SquareBracketsGroup::RESERVED_CHARACTERS,
        #endif
        #ifndef DISABLE_CBG
        CurlyBracketsGroup::RESERVED_CHARACTERS,
        #endif
        #ifndef DISABLE_QUOT
        Quotation::SINGLELINE_RESERVED_CHARACTERS,
        #endif
        #ifndef DISABLE_MULTILINE_QUOT
        Quotation::MULTILINE_RESERVED_CHARACTERS,
        #endif
        #ifndef DISABLE_PATH
        Path::RESERVED_CHARACTERS,
        #endif
        #ifndef DISABLE_ASSOC
        Association::RESERVED_CHARACTERS,
        #endif
    });

    #ifndef DISABLE_SPECIAL_ATOMS
    specialAtoms = {
        SpecialAtom(":="), // assignment symbol
        SpecialAtom("[:]"), // map literal
    };
    #endif
    for (auto atom: specialAtoms) {
        if (peekStrUntil(atom.value, terminatorCharacters, input)) {
            input.ignore(atom.value.size());
            return (WordStrictly_)move_to_heap(atom);
        }
    }

    #ifndef DISABLE_PG
    if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_convert<WordStrictly_>(consumeParenthesesGroup(input));
    }
    #endif

    #ifndef DISABLE_SBG
    if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_convert<WordStrictly_>(consumeSquareBracketsGroup(input));
    }
    #endif

    #ifndef DISABLE_CBG
    if (peekSequence(CurlyBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return mayfail_convert<WordStrictly_>(consumeCurlyBracketsGroup(input));
    }
    #ifndef DISABLE_DOLLARS_CBG
    auto dollars_cbg_seq = vec_concat({Sequence{'$'}, CurlyBracketsGroup::INITIATOR_SEQUENCE});
    if (peekSequence(dollars_cbg_seq, input)) {
        input.ignore(1); // $
        auto dollars_cbg = consumeCurlyBracketsGroup(input);
        dollars_cbg.val._dollars = true;
        if (!dollars_cbg.has_error()) {
            dollars_cbg.val._tokenLen += 1; // $
        }
        return mayfail_convert<WordStrictly_>(dollars_cbg);
    }
    #endif
    #endif

    #ifndef DISABLE_QUOT
    if (peekSequence(Quotation::DELIMITERS_SEQUENCE, input)) {
        return mayfail_convert<WordStrictly_>(consumeQuotation(input));
    }
    #ifndef DISABLE_MULTILINE_QUOT
    if (peekSequence(Quotation::ALT_DELIMITERS_SEQUENCE, input)) {
        return mayfail_convert<WordStrictly_>(consumeMultilineQuotation(input));
    }
    #endif
    #endif

    /* Atom is the "fall-through" Word */
    return mayfail_convert<WordStrictly_>(consumeAtom(terminatorCharacters, input));
}

MayFail<Word_> consumeWord(std::istringstream& input) {
    std::vector<char> terminatorCharacters;
    auto word = consumeWordStrictly(/*OUT*/terminatorCharacters, input);

    if (word.has_error()) {
        return MayFail<Word_>(variant_cast(word.val), word.err);
    }

    /* look behind */

    auto accumulatedPostfixLeftPart = std::visit(overload{
        [](Atom* atom) -> PostfixLeftPart {return atom;},
        [](Quotation* quot) -> PostfixLeftPart {return quot;},
        [](auto* mf_) -> PostfixLeftPart {return move_to_heap(unwrap(*mf_));},
    }, word.value());

    for (;;) {
        #ifndef DISABLE_PPG
        if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
            auto ppg = consumePostfixParenthesesGroup(/*OUT*/accumulatedPostfixLeftPart, input);
            if (ppg.has_error()) {
                return mayfail_convert<Word_>(ppg); // malformed postfix
            }
            continue;
        }
        #endif

        #ifndef DISABLE_PSBG
        if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
            auto psbg = consumePostfixSquareBracketsGroup(/*OUT*/accumulatedPostfixLeftPart, input);
            #ifndef DISABLE_PSBG_SUFFICES
            using Sx = PostfixSquareBracketsGroup::Suffix;
            if (peekAnyChar({Sx::EXCLAMATION_MARK, Sx::QUESTION_MARK}, input)) {
                auto& psbg = *std::get<PostfixSquareBracketsGroup*>(accumulatedPostfixLeftPart);
                psbg._suffix = Sx(input.get());
                psbg._tokenLen += 1; // ! | ?
            }
            #endif
            if (psbg.has_error()) {
                return mayfail_convert<Word_>(psbg); // malformed postfix
            }
            continue;
        }
        #endif

        #ifndef DISABLE_PATH
        if (peekSequence(Path::SEPARATOR_SEQUENCE, input)) {
            auto path = consumePath(/*OUT*/accumulatedPostfixLeftPart, terminatorCharacters, input);
            if (path.has_error()) {
                return mayfail_convert<Word_>(path); // malformed postfix
            }
            continue;
        }
        #endif

        break;
    }

    #ifndef DISABLE_ASSOC
    if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
        auto assoc = consumeAssociation(accumulatedPostfixLeftPart, input);
        return mayfail_convert<Word_>(assoc); /*
            early return assoc (malformed or not).
            Association can contain a PostfixLeftPart..
            .., but not the other way around! (precedence rule)
        */
    }
    #endif

    return std::visit(overload{
        [](Atom* atom) -> Word_ {return atom;},
        [](Quotation* quot) -> Word_ {return quot;},
        [](auto* other) -> Word_ {return move_to_heap(wrap(*other));},
    }, accumulatedPostfixLeftPart);
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

ProgramWord unwrap_pw(const ProgramWord_& pw_) {
    return std::visit(overload{
        [](Atom* atom) -> ProgramWord {return atom;},
        [](Quotation* quot) -> ProgramWord {return quot;},
        [](auto pw_) -> ProgramWord {return move_to_heap(unwrap(*pw_));}
    }, pw_);
}

Word unwrap_w(const Word_& word_) {
    return std::visit(overload{
        [](Atom* atom) -> Word {return atom;},
        [](Quotation* quot) -> Word {return quot;},
        [](auto word_) -> Word {return move_to_heap(unwrap(*word_));}
    }, word_);
}

ProgramWord_ wrap_pw(const ProgramWord& pw) {
    return std::visit(overload{
        [](Atom* atom) -> ProgramWord_ {return atom;},
        [](Quotation* quot) -> ProgramWord_ {return quot;},
        [](auto pw) -> ProgramWord_ {return move_to_heap(wrap(*pw));}
    }, pw);
}

Word_ wrap_w(const Word& word) {
    return std::visit(overload{
        [](Atom* atom) -> Word_ {return atom;},
        [](Quotation* quot) -> Word_ {return quot;},
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
