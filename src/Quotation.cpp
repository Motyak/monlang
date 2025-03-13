#include <monlang/Quotation.h>

/* impl only */
#include <monlang/common.h>
#include <monlang/ProgramSentence.h> // indentLevel global var
#include <monlang/SquareBracketsGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/loop-utils.h>
#include <utils/vec-utils.h>
#include <utils/defer-util.h>
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

MayFail<MayFail_<Quotation>> consumeQuotationStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    RECORD_INPUT_STREAM_PROGRESS();
    const std::vector<char> terminatorCharacters = {
        sequenceFirstChar(Quotation::DELIMITERS_SEQUENCE).value(),
    };

    if (!consumeSequence(Quotation::DELIMITERS_SEQUENCE, input)) {
        return Malformed(MayFail_<Quotation>{}, ERR(053));
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
        auto malformed = Malformed(MayFail_<Quotation>{quoted}, ERR(530));
        malformed.val._tokenLen = GET_INPUT_STREAM_PROGRESS() + 1; // for err reporting
        return malformed;
    }

    auto quot = MayFail_<Quotation>{quoted};
    quot._tokenLen = GET_INPUT_STREAM_PROGRESS();
    return quot;
}

static std::string consumeQuotedLine(std::istringstream&);

MayFail<MayFail_<Quotation>> consumeMultilineQuotationStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    RECORD_INPUT_STREAM_PROGRESS();
    GLOBAL indentLevel;
    auto blockIndentSeq = indentLevel * ProgramSentence::TAB_SEQUENCE;
    auto quotIndentSeq = (indentLevel + 1) * ProgramSentence::TAB_SEQUENCE;
    auto indentedTerminatorSeq = vec_concat({
        blockIndentSeq, Quotation::ALT_DELIMITERS_SEQUENCE
    });

    if (!consumeSequence(Quotation::ALT_DELIMITERS_SEQUENCE, input)) {
        return Malformed(MayFail_<Quotation>{}, ERR(054));
    }

    if (!consumeSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)
            || peekSequence(indentedTerminatorSeq, input)) {
        auto malformed = Malformed(MayFail_<Quotation>{}, ERR(541));
        malformed.val._tokenLen = GET_INPUT_STREAM_PROGRESS() + 1; // for err reporting
        return malformed;
    }

    std::vector<std::string> quotedLines;

    until (input.peek() == EOF || peekSequence(indentedTerminatorSeq, input)) {
        if (input.peek() != NEWLINE && !consumeSequence(quotIndentSeq, input)) {
            auto quoted = join(quotedLines, NEWLINE);
            auto malformed = Malformed(MayFail_<Quotation>{quoted}, ERR(542));
            malformed.val._tokenLen = GET_INPUT_STREAM_PROGRESS() + 1; // for err reporting
            return malformed;
        }
        quotedLines.push_back(consumeQuotedLine(input));
    }

    if (!consumeSequence(indentedTerminatorSeq, input)) {
        auto quoted = join(quotedLines, NEWLINE);
        auto malformed = Malformed(MayFail_<Quotation>{quoted}, ERR(540));
        malformed.val._tokenLen = GET_INPUT_STREAM_PROGRESS() + 1; // for err reporting
        return malformed;
    }

    auto quoted = join(quotedLines, NEWLINE);
    auto quot = MayFail_<Quotation>{quoted};
    quot._tokenLen = GET_INPUT_STREAM_PROGRESS();
    quot._multiline = true;
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
        return mayfail_convert<MayFail_<Quotation>*>(quot);
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

    return std::visit(
        [](auto word) -> consumeQuotation_RetType {return move_to_heap(wrap(*word));},
        accumulatedPostfixLeftPart
    );
}

consumeQuotation_RetType consumeMultilineQuotation(std::istringstream& input) {
    auto quot = consumeMultilineQuotationStrictly(input);

    if (quot.has_error()) {
        return mayfail_convert<MayFail_<Quotation>*>(quot);
    }

    /* look behind */

    using PostfixLeftPart = std::variant<Quotation*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap((Quotation)quot);

    for (;;) {
        #ifndef DISABLE_PSBG_IN_QUOT
        #ifndef DISABLE_PSBG_IN_MULTILINE_QUOT
        if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
            auto psbg = consumePostfixSquareBracketsGroup(&accumulatedPostfixLeftPart, input);
            if (psbg.has_error()) {
                return psbg; // malformed postfix
            }
            continue;
        }
        #endif
        #endif

        break;
    }

    return std::visit(
        [](auto word) -> consumeQuotation_RetType {return move_to_heap(wrap(*word));},
        accumulatedPostfixLeftPart
    );
}

MayFail_<Quotation>::MayFail_(const std::string& quoted) : quoted(quoted){}

MayFail_<Quotation>::MayFail_(Quotation quot) {
    this->quoted = quot.quoted;
    this->_tokenLen = quot._tokenLen;
    this->_multiline = quot._multiline;
}

MayFail_<Quotation>::operator Quotation() const {
    Quotation quot;
    quot.quoted = this->quoted;
    quot._tokenLen = this->_tokenLen;
    quot._multiline = this->_multiline;
    return quot;
}
