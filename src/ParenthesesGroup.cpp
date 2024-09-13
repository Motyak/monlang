#include <monlang/ParenthesesGroup.h>

#define until(x) while(!(x))

const Sequence ParenthesesGroup::INITIATOR_SEQUENCE = {'('};
const Sequence ParenthesesGroup::CONTINUATOR_SEQUENCE = {',', SPACE};
const Sequence ParenthesesGroup::TERMINATOR_SEQUENCE = {')'};

const std::vector<char> ParenthesesGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<ParenthesesGroup> consumeParenthesesGroup(std::istringstream& input) {
    std::cerr << "DEBUG consumeParenthesesGroup: `" << input.str().substr(input.tellg()) << "`" << std::endl;
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value()
    };

    if (!consumeSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ParenthesesGroup{}, Error{43}));
    }
    if (peekSequence(ParenthesesGroup::CONTINUATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ParenthesesGroup{}, Error{431}));
    }
    if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ParenthesesGroup::TERMINATOR_SEQUENCE));
        return ParenthesesGroup{};
    }

    std::vector<MayFail<Term>> terms;
    MayFail<Term> currentTerm;

    std::vector<char> termTerminatorChars = {
        sequenceFirstChar(ParenthesesGroup::CONTINUATOR_SEQUENCE).value(),
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value()
    };
    currentTerm = consumeTerm(termTerminatorChars, input);
    terms.push_back(currentTerm);
    if (!currentTerm.has_value()) {
        return std::unexpected(Malformed(ParenthesesGroup{terms}, Error{439}));
    }

    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        
        if (!consumeSequence(ParenthesesGroup::CONTINUATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(ParenthesesGroup{terms}, Error{403}));
        }
        // if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        //     return std::unexpected(Malformed(ParenthesesGroup{terms}, Error{432}));
        // }

        currentTerm = consumeTerm(termTerminatorChars, input);
        terms.push_back(currentTerm);
        if (!currentTerm.has_value()) {
            return std::unexpected(Malformed(ParenthesesGroup{terms}, Error{439}));
        }
    }

    if (!consumeSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ParenthesesGroup{terms}, Error{430}));
    }

    return ParenthesesGroup{terms};
}
