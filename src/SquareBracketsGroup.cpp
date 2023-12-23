#include <SquareBracketsGroup.h>
#include <ParenthesesGroup.h>
#include <PostfixParenthesesGroup.h>
#include <PostfixSquareBracketsGroup.h>
#include <Association.h>
#include <common.h>
#include <utils/variant-utils.h>
#include <utils/assert-utils.h>
#include <utils/str-utils.h>

#include <iostream>

const std::vector<CharacterAppearance> SquareBracketsGroup::INITIATOR_SEQUENCE = { '[' };
const std::vector<CharacterAppearance> SquareBracketsGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const std::vector<CharacterAppearance> SquareBracketsGroup::TERMINATOR_SEQUENCE = { ']' };

const std::vector<char> SquareBracketsGroup::RESERVED_CHARACTERS = {
    firstChar(INITIATOR_SEQUENCE),
    firstChar(CONTINUATOR_SEQUENCE),
    firstChar(TERMINATOR_SEQUENCE),
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
        firstChar(SquareBracketsGroup::CONTINUATOR_SEQUENCE),
        firstChar(SquareBracketsGroup::TERMINATOR_SEQUENCE)
    };
    Term currentTerm;
    try {
        currentTerm = consumeTerm(input, terminatorCharacters);
    } catch (std::runtime_error& e) {
        std::cerr << "was expecting end of square brackets group" 
                << " but got " << str(input.peek()) << std::endl;
        throw new std::runtime_error("user exception");
    }
    terms.push_back(currentTerm);
    while (input.peek() != EOF && !peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        consumeSequence(SquareBracketsGroup::CONTINUATOR_SEQUENCE, input);
        if (peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
            std::cerr << "was expecting another word" << std::endl;
            throw std::runtime_error("user exception");
        }
        try {
            currentTerm = consumeTerm(input, terminatorCharacters);
        } catch (std::runtime_error& e) {
            std::cerr << "was expecting end of square brackets group" << std::endl;
            throw new std::runtime_error("user exception");
        }
        terms.push_back(currentTerm);
    }

    if (!peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        auto& ts = SquareBracketsGroup::TERMINATOR_SEQUENCE;
        std::cerr << "was expecting " << str(ts)
                << " but got " << str(input.peek()) << std::endl;
        throw std::runtime_error("user exception");
        
    }
    input.ignore(SquareBracketsGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters

    return new SquareBracketsGroup{terms};
}
