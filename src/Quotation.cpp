#include <monlang/Quotation.h>

/* impl only */
#include <monlang/common.h>

#include <utils/loop-utils.h>

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
        NEWLINE,
        sequenceFirstChar(Quotation::DELIMITERS_SEQUENCE).value(),
    };

    if (!consumeSequence(Quotation::DELIMITERS_SEQUENCE, input)) {
        return Malformed(MayFail_<Quotation>{}, ERR(053));
    }

    std::string quoted;
    char currentChar;
    bool escaped = false;

    until (input.peek() == EOF || (!escaped && peekAnyChar(terminatorCharacters, input))) {
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
        return Malformed(MayFail_<Quotation>{quoted}, ERR(530));
    }

    auto quot = MayFail_<Quotation>{quoted};
    quot._tokenLen = GET_INPUT_STREAM_PROGRESS();
    return quot;
}
#include <utils/assert-utils.h>
MayFail<MayFail_<Quotation>> consumeMultilineQuotationStrictly(std::istringstream& input) {
    // TRACE_CUR_FUN();
    // RECORD_INPUT_STREAM_PROGRESS();
    (void)input;
    TODO();
}


consumeQuotation_RetType consumeQuotation(std::istringstream& input) {
    auto quot = consumeQuotationStrictly(input);

    //TODO: tmp
    return mayfail_convert<MayFail_<Quotation>*>(quot);
}

consumeQuotation_RetType consumeMultilineQuotation(std::istringstream& input) {
    (void)input;
    TODO();
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
