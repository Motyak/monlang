#include <SquareBracketsGroup.h>
#include <ParenthesesGroup.h>
#include <PostfixParenthesesGroup.h>
#include <PostfixSquareBracketsGroup.h>
#include <Association.h>
#include <common.h>
#include <utils/variant-utils.h>

#include <iostream>

const std::vector<char> SquareBracketsGroup::INITIATOR_SEQUENCE = { '[' };
const std::vector<char> SquareBracketsGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const std::vector<char> SquareBracketsGroup::TERMINATOR_SEQUENCE = { ']' };

const std::vector<char> SquareBracketsGroup::RESERVED_CHARACTERS = {
    SquareBracketsGroup::INITIATOR_SEQUENCE[0],
    SquareBracketsGroup::CONTINUATOR_SEQUENCE[0],
    SquareBracketsGroup::TERMINATOR_SEQUENCE[0],
};

std::optional<std::variant<SquareBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> tryConsumeSquareBracketsGroup(std::istringstream& input) {
    auto squareBracketsGroup = tryConsumeSquareBracketsGroupStrictly(input);
    if (!squareBracketsGroup) {
        return {};
    }

    using PostfixLeftPart = std::variant<SquareBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = *squareBracketsGroup;

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

std::optional<SquareBracketsGroup*> tryConsumeSquareBracketsGroupStrictly(std::istringstream& input) {
    if (!peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return {};
    }
    input.ignore(SquareBracketsGroup::INITIATOR_SEQUENCE.size()); // consume initiator characters

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while entering a square brackets group" << std::endl;
        throw std::runtime_error("user exception");
    }

    std::vector<Term> terms;

    if (peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(SquareBracketsGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters
        return new SquareBracketsGroup{terms}; // empty
    }

    std::vector<char> terminatorCharacters = {
        SquareBracketsGroup::CONTINUATOR_SEQUENCE[0],
        SquareBracketsGroup::TERMINATOR_SEQUENCE[0]
    };
    auto currentTerm = consumeTerm(input, terminatorCharacters);
    terms.push_back(currentTerm);
    while (input.peek() != EOF && !peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        consumeSequence(SquareBracketsGroup::CONTINUATOR_SEQUENCE, input);
        if (peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
            std::cerr << "was expecting another word" << std::endl;
            throw std::runtime_error("user exception");
        }
        currentTerm = consumeTerm(input, terminatorCharacters);
        terms.push_back(currentTerm);
    }

    if (!peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        auto& ts = SquareBracketsGroup::TERMINATOR_SEQUENCE;
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
    input.ignore(SquareBracketsGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters

    return new SquareBracketsGroup{terms};
}
