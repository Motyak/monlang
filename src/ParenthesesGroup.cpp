#include <ParenthesesGroup.h>
#include <SquareBracketsGroup.h>
#include <PostfixParenthesesGroup.h>
#include <PostfixSquareBracketsGroup.h>
#include <Association.h>
#include <common.h>
#include <utils/variant-utils.h>

#include <iostream>

const std::vector<char> ParenthesesGroup::INITIATOR_SEQUENCE = { '(' };
const std::vector<char> ParenthesesGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const std::vector<char> ParenthesesGroup::TERMINATOR_SEQUENCE = { ')' };

const std::vector<char> ParenthesesGroup::RESERVED_CHARACTERS = {
    ParenthesesGroup::INITIATOR_SEQUENCE[0],
    ParenthesesGroup::CONTINUATOR_SEQUENCE[0],
    ParenthesesGroup::TERMINATOR_SEQUENCE[0],
};

std::optional<std::variant<ParenthesesGroup*, Association*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>> tryConsumeParenthesesGroup(std::istringstream& input) {
    auto parenthesesGroup = tryConsumeParenthesesGroupStrictly(input);
    if (!parenthesesGroup) {
        return {};
    }

    using PostfixLeftPart = std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = *parenthesesGroup;

    BEGIN:
    if (auto postfixParenthesesGroup = tryConsumeParenthesesGroupStrictly(input)) {
        accumulatedPostfixLeftPart = new PostfixParenthesesGroup{variant_cast(accumulatedPostfixLeftPart), *postfixParenthesesGroup};
        goto BEGIN;
    }

    if (auto postfixSquareBracketsGroup = tryConsumeSquareBracketsGroupStrictly(input)) {
        accumulatedPostfixLeftPart = new PostfixSquareBracketsGroup{variant_cast(accumulatedPostfixLeftPart), *postfixSquareBracketsGroup};
        goto BEGIN;
    }

    if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
        ProgramWordWithoutAssociation leftPart = variant_cast(accumulatedPostfixLeftPart);
        input.ignore(Association::SEPARATOR_SEQUENCE.size()); // consume association separator characters
        ProgramWord rightPart = consumeProgramWord(input);
        return new Association{leftPart, rightPart};
    }

    return variant_cast(accumulatedPostfixLeftPart);
}

std::optional<ParenthesesGroup*> tryConsumeParenthesesGroupStrictly(std::istringstream& input) {
    if (!peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return {};
    }
    input.ignore(ParenthesesGroup::INITIATOR_SEQUENCE.size()); // consume initiator characters

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while entering a square brackets group" << std::endl;
        throw std::runtime_error("user exception");
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
    while (input.peek() != EOF && !peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
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
        throw std::runtime_error("user exception");
    }
    input.ignore(ParenthesesGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters

    return new ParenthesesGroup{terms};
}
