#include <monlang/CurlyBracketsGroup.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Term.h>
#include <monlang/common.h>

#include <utils/vec-utils.h>

#define until(x) while(!(x))

const Sequence CurlyBracketsGroup::INITIATOR_SEQUENCE = {'{'};
const Sequence CurlyBracketsGroup::TERMINATOR_SEQUENCE = {'}'};
const std::vector<char> CurlyBracketsGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<CurlyBracketsGroup> consumeCurlyBracketsGroup(std::istringstream& input) {
    TRACE_CUR_FUN();
    static thread_local int indentLevel = 0;
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(CurlyBracketsGroup::TERMINATOR_SEQUENCE).value()
    };
    auto indentedTermSeq = vec_union({{{SPACE, 4 * indentLevel}}, CurlyBracketsGroup::TERMINATOR_SEQUENCE});

    if (!consumeSequence(CurlyBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(CurlyBracketsGroup{}, Error{41}));
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
        if (!consumeSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed<CurlyBracketsGroup>(CurlyBracketsTerm(term), Error{410}));
        }
        return CurlyBracketsTerm(term);
    }

    input.ignore(sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE));

    if (peekSequence(indentedTermSeq, input)) {
        return std::unexpected(Malformed(CurlyBracketsGroup{}, Error{412}));
    }

    indentLevel++;

    std::vector<MayFail<ProgramSentence>> sentences;
    MayFail<ProgramSentence> currentSentence;

    currentSentence = consumeProgramSentence(input, indentLevel);
    if (currentSentence.has_value() && currentSentence.value().programWords.size() == 0) {
        goto LOOP; // ignore empty sentences
    }
    sentences.push_back(currentSentence);
    if (!currentSentence.has_value()) {
        indentLevel--; // restore indent level, because static
        return std::unexpected(Malformed(CurlyBracketsGroup{sentences}, Error{419}));
    }

    LOOP:
    until (input.peek() == EOF || !peekSequence({{SPACE, 4 * indentLevel}}, input)) {
        currentSentence = consumeProgramSentence(input, indentLevel);
        if (currentSentence.has_value() && currentSentence.value().programWords.size() == 0) {
            continue; // ignore empty sentences
        }
        sentences.push_back(currentSentence);
        if (!currentSentence.has_value()) {
            indentLevel--; // restore indent level, because static
            return std::unexpected(Malformed(CurlyBracketsGroup{sentences}, Error{419}));
        }
    }

    if (!consumeSequence(indentedTermSeq, input)) {
        indentLevel--; // restore indent level, because static
        return std::unexpected(Malformed(CurlyBracketsGroup{sentences}, Error{410}));
    }

    if (sentences.size() == 0) {
        indentLevel--; // restore indent level, because static
        return std::unexpected(Malformed(CurlyBracketsGroup{}, Error{413}));
    }

    indentLevel--;

    return CurlyBracketsGroup{sentences};
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

CurlyBracketsGroup::CurlyBracketsGroup(std::vector<MayFail<ProgramSentence>> sentences) {
    this->sentences = sentences;
}

CurlyBracketsGroup::CurlyBracketsGroup(std::optional<MayFail<Term>> term, std::vector<MayFail<ProgramSentence>> sentences) {
    this->term = term;
    this->sentences = sentences;
}

CurlyBracketsTerm::CurlyBracketsTerm(MayFail<Term> term) : CurlyBracketsGroup{term, toSentences(term)} {}
