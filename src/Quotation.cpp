#include <monlang/Quotation.h>

/* impl only */
#include <monlang/common.h>
#include <monlang/ProgramSentence.h> // indentLevel global var

#include <utils/loop-utils.h>
#include <utils/vec-utils.h>
#include <utils/str-utils.h>

const Sequence Quotation::DELIMITERS_SEQUENCE = {'"'};

const Sequence Quotation::ALT_DELIMITERS_SEQUENCE = {{'`', 3_}};

const std::vector<char> Quotation::SINGLELINE_RESERVED_CHARACTERS = {
    sequenceFirstChar(Quotation::DELIMITERS_SEQUENCE).value(),
};

const std::vector<char> Quotation::MULTILINE_RESERVED_CHARACTERS = {
    sequenceFirstChar(Quotation::ALT_DELIMITERS_SEQUENCE).value(),
};

MayFail<Quotation> consumeQuotation(std::istringstream& input) {
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

MayFail<Quotation> consumeMultilineQuotation(std::istringstream& input) {
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
            // UPDATE 14/09/2025: we don't want any char escaping in a multiline quot
            // escaped = true;
            res += currentChar; //
        }
        else {
            res += currentChar;
        }
    }
    input.ignore(1); // consume line terminator

    return res;
}

Quotation::Quotation(const std::string& quoted) : quoted(quoted){}
