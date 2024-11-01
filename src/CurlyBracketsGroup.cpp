#include <monlang/CurlyBracketsGroup.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Term.h>
#include <monlang/common.h>

/* in impl only */
#include <monlang/ParenthesesGroup.h>
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/vec-utils.h>
#include <utils/defer-util.h>
#include <utils/variant-utils.h>

#define until(x) while(!(x))

const Sequence CurlyBracketsGroup::INITIATOR_SEQUENCE = {'{'};
const Sequence CurlyBracketsGroup::TERMINATOR_SEQUENCE = {'}'};
const std::vector<char> CurlyBracketsGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<CurlyBracketsGroup> consumeCurlyBracketsGroupStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    static thread_local int indentLevel = 0;
    auto indentedTerminatorSeq = vec_concat({INDENT_SEQUENCE(), CurlyBracketsGroup::TERMINATOR_SEQUENCE});

    if (!consumeSequence(CurlyBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(CurlyBracketsGroup{}, ERR(041)));
    }

    if (peekSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(CurlyBracketsGroup::TERMINATOR_SEQUENCE));
        return CurlyBracketsGroup{};
    }

    /* handle single term expression */
    if (!peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        std::vector<char> termTerminatorChars = {
            sequenceFirstChar(CurlyBracketsGroup::TERMINATOR_SEQUENCE).value(),
        };
        auto term = consumeTerm(termTerminatorChars, input);
        if (!term.has_value()) {
            return std::unexpected(Malformed<CurlyBracketsGroup>(CurlyBracketsTerm(term), ERR(519)));
        }
        if (!consumeSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed<CurlyBracketsGroup>(CurlyBracketsTerm(term), ERR(510)));
        }
        return CurlyBracketsTerm(term);
    }

    input.ignore(sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE));

    if (peekSequence(indentedTerminatorSeq, input)) {
        return std::unexpected(Malformed(CurlyBracketsGroup{}, ERR(412)));
    }

    indentLevel++;
    defer { indentLevel--; }; // restore indent level, because static

    std::vector<MayFail<ProgramSentence>> sentences;
    MayFail<ProgramSentence> currentSentence;

    until (input.peek() == EOF || peekSequence(indentedTerminatorSeq, input)) {
        currentSentence = consumeProgramSentence(input, indentLevel);
        if (currentSentence.has_value() && currentSentence.value().programWords.size() == 0) {
            continue; // ignore empty sentences
        }
        sentences.push_back(currentSentence);
        if (!currentSentence.has_value()) {
            return std::unexpected(Malformed(CurlyBracketsGroup{sentences}, ERR(419)));
        }
    }

    if (!consumeSequence(indentedTerminatorSeq, input)) {
        return std::unexpected(Malformed(CurlyBracketsGroup{sentences}, ERR(410)));
    }

    if (sentences.size() == 0) {
        return std::unexpected(Malformed(CurlyBracketsGroup{}, ERR(413)));
    }

    return CurlyBracketsGroup{sentences};
}

consumeCurlyBracketsGroup_RetType consumeCurlyBracketsGroup(std::istringstream& input) {
    auto cbg = consumeCurlyBracketsGroupStrictly(input);

    if (!cbg.has_value()) {
        return mayfail_convert<CurlyBracketsGroup*>(cbg);
    }

    using PostfixLeftPart = std::variant<CurlyBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap(cbg.value());

    [[maybe_unused]]
    BEGIN:
    #ifndef DISABLE_PPG_IN_CBG
    if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        auto whats_right_behind = consumeParenthesesGroupStrictly(input);
        auto curr_ppg = move_to_heap(PostfixParenthesesGroup{
            variant_cast(accumulatedPostfixLeftPart),
            whats_right_behind
        });
        if (!whats_right_behind.has_value()) {
            return std::unexpected(Malformed(curr_ppg, ERR(319)));
        }
        accumulatedPostfixLeftPart = curr_ppg;
        goto BEGIN;
    }
    #endif

    #ifndef DISABLE_PSBG_IN_CBG
    if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        auto whats_right_behind = consumeSquareBracketsGroupStrictly(input);
        auto curr_psbg = move_to_heap(PostfixSquareBracketsGroup{
            variant_cast(accumulatedPostfixLeftPart),
            whats_right_behind
        });
        if (!whats_right_behind.has_value()) {
            return std::unexpected(Malformed(curr_psbg, ERR(329)));
        }
        accumulatedPostfixLeftPart = curr_psbg;
        goto BEGIN;
    }
    #endif

    return std::visit(
        [](auto word) -> consumeCurlyBracketsGroup_RetType {return word;},
        accumulatedPostfixLeftPart
    );
}

////////////////////////////////////////////////////////////////

CurlyBracketsGroup::CurlyBracketsGroup(std::vector<MayFail<ProgramSentence>> sentences) {
    this->sentences = sentences;
}

CurlyBracketsGroup::CurlyBracketsGroup(std::vector<MayFail<ProgramSentence>> sentences, std::optional<MayFail<Term>> term) {
    this->sentences = sentences;
    this->term = term;
}

static std::vector<MayFail<ProgramSentence>> toSentences(MayFail<Term> term) {
    Term term_ = mayfail_unwrap(term);

    std::vector<MayFail<ProgramWord>> programWords;
    for (auto word: term_.words) {
        programWords.push_back(mayfail_cast<ProgramWord>(word));
    }

    if (!term.has_value()) {
        // vector containing one malformed program sentence with term error code
        return {std::unexpected(Malformed(ProgramSentence{programWords}, term.error().err))};
    }

    return {ProgramSentence{programWords}};
}

CurlyBracketsTerm::CurlyBracketsTerm(MayFail<Term> term) : CurlyBracketsGroup{toSentences(term), term} {}
