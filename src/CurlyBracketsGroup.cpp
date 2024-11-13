#include <monlang/CurlyBracketsGroup.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Term.h>
#include <monlang/common.h>

/* in impl only */
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/vec-utils.h>
#include <utils/defer-util.h>

#define until(x) while(!(x))

const Sequence CurlyBracketsGroup::INITIATOR_SEQUENCE = {'{'};
const Sequence CurlyBracketsGroup::TERMINATOR_SEQUENCE = {'}'};
const std::vector<char> CurlyBracketsGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<MayFail_<CurlyBracketsGroup>> consumeCurlyBracketsGroupStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    static thread_local int indentLevel = 0;
    auto indentedTerminatorSeq = vec_concat({INDENT_SEQUENCE(), CurlyBracketsGroup::TERMINATOR_SEQUENCE});

    if (!consumeSequence(CurlyBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<CurlyBracketsGroup>{}, ERR(041));
    }

    if (peekSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(CurlyBracketsGroup::TERMINATOR_SEQUENCE));
        return MayFail_<CurlyBracketsGroup>{};
    }

    /* handle single term expression */
    if (!peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        std::vector<char> termTerminatorChars = {
            sequenceFirstChar(CurlyBracketsGroup::TERMINATOR_SEQUENCE).value(),
        };
        auto term = consumeTerm(termTerminatorChars, input);
        if (term.has_error()) {
            return Malformed<MayFail_<CurlyBracketsGroup>>(MayFail_<CurlyBracketsTerm>(term), ERR(519));
        }
        if (!consumeSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
            return Malformed<MayFail_<CurlyBracketsGroup>>(MayFail_<CurlyBracketsTerm>(term), ERR(510));
        }
        return MayFail_<CurlyBracketsTerm>(term);
    }

    input.ignore(sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE));

    if (peekSequence(indentedTerminatorSeq, input)) {
        return Malformed(MayFail_<CurlyBracketsGroup>{}, ERR(412));
    }

    indentLevel++;
    defer { indentLevel--; }; // restore indent level, because static

    std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;
    MayFail<MayFail_<ProgramSentence>> currentSentence;

    until (input.peek() == EOF || peekSequence(indentedTerminatorSeq, input)) {
        currentSentence = consumeProgramSentence(input, indentLevel);
        if (!currentSentence.has_error() && currentSentence.value().programWords.size() == 0) {
            continue; // ignore empty sentences
        }
        sentences.push_back(currentSentence);
        if (currentSentence.has_error()) {
            return Malformed(MayFail_<CurlyBracketsGroup>{sentences}, ERR(419));
        }
    }

    if (!consumeSequence(indentedTerminatorSeq, input)) {
        return Malformed(MayFail_<CurlyBracketsGroup>{sentences}, ERR(410));
    }

    if (sentences.size() == 0) {
        return Malformed(MayFail_<CurlyBracketsGroup>{}, ERR(413));
    }

    return MayFail_<CurlyBracketsGroup>{sentences};
}

consumeCurlyBracketsGroup_RetType consumeCurlyBracketsGroup(std::istringstream& input) {
    auto cbg = consumeCurlyBracketsGroupStrictly(input);

    if (cbg.has_error()) {
        return mayfail_convert<MayFail_<CurlyBracketsGroup>*>(cbg);
    }

    /* look behind */

    using PostfixLeftPart = std::variant<CurlyBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap((CurlyBracketsGroup)cbg);

    for (;;) {
        #ifndef DISABLE_PPG_IN_CBG
        if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
            auto ppg = consumePostfixParenthesesGroup(&accumulatedPostfixLeftPart, input);
            if (ppg.has_error()) {
                return ppg; // malformed postfix
            }
            continue;
        }
        #endif

        #ifndef DISABLE_PSBG_IN_CBG
        if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
            auto psbg = consumePostfixSquareBracketsGroup(&accumulatedPostfixLeftPart, input);
            if (psbg.has_error()) {
                return psbg; // malformed postfix
            }
            continue;
        }
        #endif

        break;
    }

    return std::visit(
        [](auto word) -> consumeCurlyBracketsGroup_RetType {return move_to_heap(word->wrap());},
        accumulatedPostfixLeftPart
    );
}

////////////////////////////////////////////////////////////////

CurlyBracketsGroup::CurlyBracketsGroup(std::vector<ProgramSentence> sentences) {
    this->sentences = sentences;
}

CurlyBracketsGroup::CurlyBracketsGroup(std::vector<ProgramSentence> sentences, std::optional<Term> term) {
    this->sentences = sentences;
    this->term = term;
}

static std::vector<ProgramSentence> toSentences(Term term) {
    std::vector<ProgramWord> programWords;
    for (auto word: term.words) {
        programWords.push_back(variant_cast(word));
    }
    return {ProgramSentence{programWords}};
}

CurlyBracketsTerm::CurlyBracketsTerm(Term term) : CurlyBracketsGroup{toSentences(term), term} {}

///////////////////////////////////////////////////////////

MayFail_<CurlyBracketsGroup> CurlyBracketsGroup::wrap() const {
    MayFail_<CurlyBracketsGroup> res;
    for (auto e: this->sentences) {
        res.sentences.push_back(e.wrap());
    }
    if (term.has_value()) {
        res.term = term.value().wrap();
    }
    return res;
}

MayFail_<CurlyBracketsGroup>::MayFail_(std::vector<MayFail<MayFail_<ProgramSentence>>> sentences) : sentences(sentences){}

MayFail_<CurlyBracketsGroup>::MayFail_(std::vector<MayFail<MayFail_<ProgramSentence>>> sentences, std::optional<MayFail<MayFail_<Term>>> term) : sentences(sentences), term(term){}

MayFail_<CurlyBracketsGroup>::MayFail_(CurlyBracketsGroup cbg) {
    *this = cbg.wrap();
}

MayFail_<CurlyBracketsGroup>::operator CurlyBracketsGroup() const {
    CurlyBracketsGroup res;
    for (auto e: this->sentences) {
        res.sentences.push_back(e.value().unwrap());
    }
    if (term.has_value()) {
        res.term = term.value().val.unwrap();
    }
    return res;
}

CurlyBracketsGroup MayFail_<CurlyBracketsGroup>::unwrap() const {
    return (CurlyBracketsGroup)*this;
}

static std::vector<MayFail<MayFail_<ProgramSentence>>> toSentences(MayFail<MayFail_<Term>> term) {
    std::vector<MayFail<ProgramWord_>> programWords;
    for (auto word: term.val.words) {
        programWords.push_back(mayfail_cast<ProgramWord_>(word));
    }
    return {MayFail_<ProgramSentence>{programWords}};
}

MayFail_<CurlyBracketsTerm>::MayFail_(MayFail<MayFail_<Term>> term) : MayFail_<CurlyBracketsGroup>{toSentences(term), term} {}
