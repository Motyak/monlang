#include <monlang/Quotation.h>

/* impl only */
#include <monlang/common.h>
#include <monlang/ProgramSentence.h> // indentLevel global var
#include <monlang/SquareBracketsGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/loop-utils.h>
#include <utils/vec-utils.h>
#include <utils/str-utils.h>
#include <utils/mem-utils.h>

const Sequence Quotation::DELIMITERS_SEQUENCE = {'"'};

const Sequence Quotation::ALT_DELIMITERS_SEQUENCE = {{'`', 3_}};

const std::vector<char> Quotation::SINGLELINE_RESERVED_CHARACTERS = {
    sequenceFirstChar(Quotation::DELIMITERS_SEQUENCE).value(),
};

const std::vector<char> Quotation::MULTILINE_RESERVED_CHARACTERS = {
    sequenceFirstChar(Quotation::ALT_DELIMITERS_SEQUENCE).value(),
};

MayFail<Quotation> consumeQuotationStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    RECORD_INPUT_STREAM_PROGRESS();
    const std::vector<char> terminatorCharacters = {
        sequenceFirstChar(Quotation::DELIMITERS_SEQUENCE).value(),
    };

    if (!consumeSequence(Quotation::DELIMITERS_SEQUENCE, input)) {
        return Malformed(Quotation{}, ERR(053));
    }

    std::string quoted;
    char currentChar;
    bool escaped = false;

    until (input.peek() == EOF || input.peek() == NEWLINE \
            || (!escaped && peekAnyChar(terminatorCharacters, input))) {
        input.get(currentChar);
        if (escaped) {
            quoted += currentChar == 'n'? '\n' : currentChar;
            escaped = false;
        }
        else if (currentChar == BACKSLASH) {
            escaped = true;
        }
        else {
            quoted += currentChar;
        }
    }

    if (!consumeSequence(Quotation::DELIMITERS_SEQUENCE, input)) {
        auto malformed = Malformed(Quotation{quoted}, ERR(530));
        malformed.val._tokenLen = GET_INPUT_STREAM_PROGRESS(); // for err reporting
        return malformed;
    }

    auto quot = Quotation{quoted};
    quot._tokenLen = GET_INPUT_STREAM_PROGRESS();
    return quot;
}

static std::string consumeQuotedLine(std::istringstream&);

MayFail<Quotation> consumeMultilineQuotationStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    RECORD_INPUT_STREAM_PROGRESS();
    GLOBAL indentLevel;
    auto blockIndentSeq = indentLevel * ProgramSentence::TAB_SEQUENCE;
    auto quotIndentSeq = (indentLevel + 1) * ProgramSentence::TAB_SEQUENCE;
    auto indentedTerminatorSeq = vec_concat({
        blockIndentSeq, Quotation::ALT_DELIMITERS_SEQUENCE
    });

    if (!consumeSequence(Quotation::ALT_DELIMITERS_SEQUENCE, input)) {
        return Malformed(Quotation{}, ERR(054));
    }

    if (!consumeSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)
            || peekSequence(indentedTerminatorSeq, input)) {
        auto malformed = Malformed(Quotation{}, ERR(541));
        malformed.val._tokenLen = GET_INPUT_STREAM_PROGRESS(); // for err reporting
        return malformed;
    }

    std::vector<std::string> quotedLines;

    until (input.peek() == EOF || peekSequence(indentedTerminatorSeq, input)) {
        if (input.peek() != NEWLINE && !consumeSequence(quotIndentSeq, input)) {
            auto quoted = join(quotedLines, NEWLINE);
            auto malformed = Malformed(Quotation{quoted}, ERR(542));
            malformed.val._tokenLen = GET_INPUT_STREAM_PROGRESS(); // for err reporting
            return malformed;
        }
        quotedLines.push_back(consumeQuotedLine(input));
    }

    if (!consumeSequence(indentedTerminatorSeq, input)) {
        auto quoted = join(quotedLines, NEWLINE);
        auto malformed = Malformed(Quotation{quoted}, ERR(540));
        malformed.val._tokenLen = GET_INPUT_STREAM_PROGRESS(); // for err reporting
        return malformed;
    }

    auto quoted = join(quotedLines, NEWLINE);
    auto quot = Quotation{quoted};
    quot._tokenLen = GET_INPUT_STREAM_PROGRESS();
    return quot;
}

std::string consumeQuotedLine(std::istringstream& input) {
    std::string res;
    char currentChar;
    bool escaped = false;

    until (input.peek() == EOF || input.peek() == NEWLINE) {
        input.get(currentChar);
        if (escaped) {
            res += currentChar; // no special character in multiline quot
            escaped = false;
        }
        else if (currentChar == BACKSLASH) {
            escaped = true;
        }
        else {
            res += currentChar;
        }
    }
    input.ignore(1); // consume line terminator

    return res;
}

consumeQuotation_RetType consumeQuotation(std::istringstream& input) {
    auto quot = consumeQuotationStrictly(input);

    if (quot.has_error()) {
        return mayfail_convert<Quotation*>(quot);
    }

    /* look behind */

    using PostfixLeftPart = std::variant<Quotation*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap((Quotation)quot);

    for (;;) {
        #ifndef DISABLE_PSBG_IN_QUOT
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

    return std::visit(overload{
        [](Quotation* quot) -> consumeQuotation_RetType {return quot;},
        [](PostfixSquareBracketsGroup* psbg) -> consumeQuotation_RetType {return move_to_heap(wrap(*psbg));},
    }, accumulatedPostfixLeftPart);
}

consumeQuotation_RetType consumeMultilineQuotation(std::istringstream& input) {
    auto quot = consumeMultilineQuotationStrictly(input);

    if (quot.has_error()) {
        return mayfail_convert<Quotation*>(quot);
    }

    /* look behind */

    using PostfixLeftPart = std::variant<Quotation*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap((Quotation)quot);

    for (;;) {
        #if !defined DISABLE_PSBG_IN_QUOT && !defined DISABLE_PSBG_IN_MULTILINE_QUOT
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

    return std::visit(overload{
        [](Quotation* quot) -> consumeQuotation_RetType {return quot;},
        [](PostfixSquareBracketsGroup* psbg) -> consumeQuotation_RetType {return move_to_heap(wrap(*psbg));},
    }, accumulatedPostfixLeftPart);
}

Quotation::Quotation(const std::string& quoted) : quoted(quoted){}
