#include <Quotation.h>
#include <utils/str-utils.h>

#include <iostream>
#include <algorithm>

const std::vector<CharacterAppearance> Quotation::INITIATOR_SEQUENCE = { '"' };
const std::vector<CharacterAppearance> Quotation::TERMINATOR_SEQUENCE = { {BACKSLASH, 0}, '"' };
const std::vector<CharacterAppearance> Quotation::ALT_INITIATOR_SEQUENCE = { {'`', 3} };
const std::vector<CharacterAppearance> Quotation::ALT_TERMINATOR_SEQUENCE = { {NEWLINE}, {'`', 3} };

const std::vector<char> Quotation::RESERVED_CHARACTERS = {
    firstChar(INITIATOR_SEQUENCE),
    firstChar(TERMINATOR_SEQUENCE),
    firstChar(ALT_INITIATOR_SEQUENCE),
    firstChar(ALT_TERMINATOR_SEQUENCE),
};

std::optional<MayFail<std::variant<Quotation*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>>> tryConsumeQuotation(std::istringstream& input) {
    //TODO: 

    auto quotation = tryConsumeQuotationStrictly(input);
    if (!quotation) {
        return {};
    }

    return *quotation;
}

static Quotation* consumeOnelineQuotation(std::istringstream&);
static Quotation* consumeMultilineQuotation(std::istringstream&);

std::optional<MayFail<Quotation*>> tryConsumeQuotationStrictly(std::istringstream& input) {
    if (peekSequence(Quotation::INITIATOR_SEQUENCE, input)) {
        return consumeOnelineQuotation(input);
    }

    if (peekSequence(Quotation::ALT_INITIATOR_SEQUENCE, input)) {
        return consumeMultilineQuotation(input);
    }

    return {};
}

Quotation* consumeOnelineQuotation(std::istringstream& input) {
    input.ignore(sequenceLen(Quotation::INITIATOR_SEQUENCE)); // consume initiator characters

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while entering a oneline quotation" << std::endl;
        throw std::runtime_error("user exception");
    }

    Quoted quoted;
    if (peekSequence(Quotation::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(Quotation::TERMINATOR_SEQUENCE)); // consume terminator characters
        return new Quotation{quoted}; // empty
    }
    quoted = consumeOnelineQuoted(input);

    if (!peekSequence(Quotation::TERMINATOR_SEQUENCE, input)) {
        auto& ts = Quotation::TERMINATOR_SEQUENCE;
        std::cerr << "was expecting " << str(ts)
                << " but got " << str(input.peek()) << std::endl;
        throw std::runtime_error("user exception");
        
    }
    input.ignore(sequenceLen(Quotation::TERMINATOR_SEQUENCE)); // consume terminator characters

    return new Quotation{quoted};
}

static QuotedFormat consumeQuotedFormat(std::istringstream&);

Quotation* consumeMultilineQuotation(std::istringstream& input) {
    input.ignore(sequenceLen(Quotation::ALT_INITIATOR_SEQUENCE)); // consume initiator characters

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while entering a multiline quotation" << std::endl;
        throw std::runtime_error("user exception");
    }

    auto quotedFormat = consumeQuotedFormat(input);
    input.ignore(1); // consume newline

    Quoted quoted;
    if (peekSequence(Quotation::ALT_TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(Quotation::ALT_TERMINATOR_SEQUENCE)); // consume terminator characters
        return new Quotation{quoted}; // empty
    }
    quoted = consumeMultilineQuoted(input);

    if (!peekSequence(Quotation::ALT_TERMINATOR_SEQUENCE, input)) {
        auto& ts = Quotation::ALT_TERMINATOR_SEQUENCE;
        std::cerr << "was expecting " << str(ts)
                << " but got " << str(input.peek()) << std::endl;
        throw std::runtime_error("user exception");
        
    }
    input.ignore(sequenceLen(Quotation::ALT_TERMINATOR_SEQUENCE)); // consume terminator characters

    return new Quotation{quoted, quotedFormat};
}

QuotedFormat consumeQuotedFormat(std::istringstream& input) {
    static const std::vector<std::vector<CharacterAppearance>> TERMINATOR_SEQUENCES = {
        {NEWLINE}
    };

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while entering a quoted format" << std::endl;
        throw std::runtime_error("user exception");
    }

    QuotedFormat quotedFormat = "";
    char currentChar;
    while (input.peek() != EOF && std::all_of(
            TERMINATOR_SEQUENCES.begin(),
            TERMINATOR_SEQUENCES.end(),
            [&input](auto seq){return !peekSequence(seq, input);})) {
        input.get(currentChar);
        quotedFormat += currentChar;
    }

    if (input.peek() != NEWLINE) {
        std::cerr << "was expecting " << str(NEWLINE)
                << " but got " << str(input.peek()) << std::endl;
        throw std::runtime_error("user exception");
    }

    return quotedFormat;
}
