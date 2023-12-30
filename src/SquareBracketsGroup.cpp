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
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

static MayFail<std::variant<SquareBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> lookBehind(SquareBracketsGroup*, std::istringstream&);

std::optional<MayFail<std::variant<SquareBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>>> tryConsumeSquareBracketsGroup(std::istringstream& input) {
    auto squareBracketsGroup = tryConsumeSquareBracketsGroupStrictly(input);

    if (!squareBracketsGroup) {
        return {};
    }

    return squareBracketsGroup->and_then(lookBehind);
}

MayFail<std::variant<SquareBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> lookBehind(SquareBracketsGroup* squareBracketsGroup, std::istringstream& input) {
    using PostfixLeftPart = std::variant<SquareBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = squareBracketsGroup;

    BEGIN:
    if (auto postfixParenthesesGroup = tryConsumeParenthesesGroupStrictly(input)) {
        if (!*postfixParenthesesGroup) {
            return std::unexpected(Error{102, new Error{postfixParenthesesGroup->error()}});
        }
        accumulatedPostfixLeftPart = new PostfixParenthesesGroup{variant_cast(accumulatedPostfixLeftPart), **postfixParenthesesGroup};
        goto BEGIN;
    }

    if (auto postfixSquareBracketsGroup = tryConsumeSquareBracketsGroupStrictly(input)) {
        if (!*postfixSquareBracketsGroup) {
            return std::unexpected(Error{103, new Error{postfixSquareBracketsGroup->error()}});
        }
        accumulatedPostfixLeftPart = new PostfixSquareBracketsGroup{variant_cast(accumulatedPostfixLeftPart), **postfixSquareBracketsGroup};
        goto BEGIN;
    }

    if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
        ProgramWordWithoutAssociation leftPart = variant_cast(accumulatedPostfixLeftPart);
        input.ignore(sequenceLen(Association::SEPARATOR_SEQUENCE)); // consume association separator characters
        if (auto rightPart = consumeProgramWord(input); !rightPart) {
            return std::unexpected(Error{104, new Error{rightPart.error()}});
        } else {
            return new Association{leftPart, *rightPart};
        }
    }

    return variant_cast(accumulatedPostfixLeftPart);
}

std::optional<MayFail<SquareBracketsGroup*>> tryConsumeSquareBracketsGroupStrictly(std::istringstream& input) {
    if (!peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return {};
    }
    input.ignore(sequenceLen(SquareBracketsGroup::INITIATOR_SEQUENCE)); // consume initiator characters

    if (input.peek() == EOF) {
        return std::unexpected(Error{105});
    }

    std::vector<Term> terms;

    if (peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(SquareBracketsGroup::TERMINATOR_SEQUENCE)); // consume terminator characters
        return new SquareBracketsGroup{terms}; // empty
    }

    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(SquareBracketsGroup::CONTINUATOR_SEQUENCE).value(),
        sequenceFirstChar(SquareBracketsGroup::TERMINATOR_SEQUENCE).value()
    };

    if (auto currentTerm = consumeTerm(input, terminatorCharacters); !currentTerm) {
        return std::unexpected(Error{106, new Error{currentTerm.error()}});
    } else {
        terms.push_back(*currentTerm);
    }
    while (input.peek() != EOF && !peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        if (auto err = consumeSequence(SquareBracketsGroup::CONTINUATOR_SEQUENCE, input)) {
            return std::unexpected(Error{107, new Error{err.error()}});
        }
        if (peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
            return std::unexpected(Error{108});
        }
        if (auto currentTerm = consumeTerm(input, terminatorCharacters); !currentTerm) {
            return std::unexpected(Error{106, new Error{currentTerm.error()}});
        } else {
            terms.push_back(*currentTerm);
        }
    }

    if (!peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Error{109});        
    }
    input.ignore(sequenceLen(SquareBracketsGroup::TERMINATOR_SEQUENCE)); // consume terminator characters

    return new SquareBracketsGroup{terms};
}
