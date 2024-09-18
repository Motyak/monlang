#include <monlang/CurlyBracketsGroup.h>
#include <monlang/common.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Term.h>

#define until(x) while(!(x))

const Sequence CurlyBracketsGroup::INITIATOR_SEQUENCE = {'{'};
const Sequence CurlyBracketsGroup::TERMINATOR_SEQUENCE = {'}'};
const std::vector<char> CurlyBracketsGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

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

CurlyBracketsTerm::CurlyBracketsTerm(MayFail<Term> term) : CurlyBracketsGroup{toSentences(term)} {}

MayFail<CurlyBracketsGroup> consumeCurlyBracketsGroup(int indentLevel, std::istringstream& input) {
    TRACE_CUR_FUN();
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(CurlyBracketsGroup::TERMINATOR_SEQUENCE).value()
    };

    if (!consumeSequence(CurlyBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(CurlyBracketsGroup{}, Error{41}));
    }

    if (peekSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(CurlyBracketsGroup::TERMINATOR_SEQUENCE));
        return CurlyBracketsGroup{};
    }

    /* handle single term expression */
    if (!peekSequence({NEWLINE}, input)) {
        std::vector<char> termTerminatorChars = {
            sequenceFirstChar(CurlyBracketsGroup::TERMINATOR_SEQUENCE).value(),
        };
        auto term = consumeTerm(termTerminatorChars, input);
        return CurlyBracketsTerm(term);
    }

    input.ignore(1); // consume newline

    std::vector<MayFail<ProgramSentence>> sentences;
    MayFail<ProgramSentence> currentSentence;

    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        if (!consumeSequence({{SPACE, 4 * indentLevel}}, input)) {
            return std::unexpected(Malformed(CurlyBracketsGroup{sentences}, Error{411}));
        }
        currentSentence = consumeProgramSentence(input);
        sentences.push_back(currentSentence);
        if (!currentSentence.has_value()) {
            return std::unexpected(Malformed(CurlyBracketsGroup{sentences}, Error{419}));
        }
    }

    if (!consumeSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(CurlyBracketsGroup{sentences}, Error{430}));
    }

    return CurlyBracketsGroup{sentences};
}
