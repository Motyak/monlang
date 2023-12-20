#include <ParenthesesGroup.h>
#include <common.h>

#include <iostream>

const std::vector<char> ParenthesesGroup::INITIATOR_SEQUENCE = { '(' };
const std::vector<char> ParenthesesGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const std::vector<char> ParenthesesGroup::TERMINATOR_SEQUENCE = { ')' };

const std::vector<char> ParenthesesGroup::RESERVED_CHARACTERS = {
    ParenthesesGroup::INITIATOR_SEQUENCE[0],
    ParenthesesGroup::CONTINUATOR_SEQUENCE[0],
    ParenthesesGroup::TERMINATOR_SEQUENCE[0],
};

std::optional<ParenthesesGroup*> tryConsumeParenthesesGroupStrictly(std::istringstream& input) {
    if (!peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return {};
    }
    input.ignore(ParenthesesGroup::INITIATOR_SEQUENCE.size()); // consume initiator characters

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while entering a square brackets group" << std::endl;
        exit(1);
    }

    std::vector<Term> terms;

    if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(ParenthesesGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters
        return new ParenthesesGroup{terms}; // empty
    }

    auto terminatorSequences = {
        ParenthesesGroup::CONTINUATOR_SEQUENCE[0],
        ParenthesesGroup::TERMINATOR_SEQUENCE[0]
    };
    auto currentTerm = consumeTerm(input, terminatorSequences);
    terms.push_back(currentTerm);
    while (input && !peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        consumeSequence(ParenthesesGroup::CONTINUATOR_SEQUENCE, input);
        currentTerm = consumeTerm(input, terminatorSequences);
        terms.push_back(currentTerm);
    }

    if (!peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        auto& ts = ParenthesesGroup::TERMINATOR_SEQUENCE;
        if (input.peek() == -1) {
            std::cerr << "was expecting `" 
                << std::string(ts.begin(), ts.end()) 
                << "` but hit EOF" << std::endl;
        } else {
            std::cerr << "was expecting `" 
                << std::string(ts.begin(), ts.end()) 
                << "` but found `" << char(input.peek()) << "`" << std::endl;
        }
        exit(1);
    }
    input.ignore(ParenthesesGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters

    return new ParenthesesGroup{terms};
}
