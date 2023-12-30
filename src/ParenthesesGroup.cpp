#include <ParenthesesGroup.h>
#include <SquareBracketsGroup.h>
#include <PostfixParenthesesGroup.h>
#include <PostfixSquareBracketsGroup.h>
#include <Association.h>
#include <common.h>
#include <utils/variant-utils.h>
#include <utils/str-utils.h>

#include <iostream>

const Sequence ParenthesesGroup::INITIATOR_SEQUENCE = { '(' };
const Sequence ParenthesesGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const Sequence ParenthesesGroup::TERMINATOR_SEQUENCE = { ')' };

const std::vector<char> ParenthesesGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

static MayFail<std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> lookBehind(ParenthesesGroup*, std::istringstream&);

std::optional<MayFail<std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>>> tryConsumeParenthesesGroup(std::istringstream& input) {
    auto parenthesesGroup = tryConsumeParenthesesGroupStrictly(input);

    if (!parenthesesGroup) {
        return {};
    }

    return parenthesesGroup->and_then([&input](auto pg){return lookBehind(pg, input);});
}

MayFail<std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> lookBehind(ParenthesesGroup* parenthesesGroup, std::istringstream& input) {
    using PostfixLeftPart = std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = parenthesesGroup;

    BEGIN:
    if (auto postfixParenthesesGroup = tryConsumeParenthesesGroupStrictly(input)) {
        if (!*postfixParenthesesGroup) {
            return std::unexpected(Error{119, new Error{postfixParenthesesGroup->error()}});
        }
        accumulatedPostfixLeftPart = new PostfixParenthesesGroup{variant_cast(accumulatedPostfixLeftPart), **postfixParenthesesGroup};
        goto BEGIN;
    }

    if (auto postfixSquareBracketsGroup = tryConsumeSquareBracketsGroupStrictly(input)) {
        if (!*postfixSquareBracketsGroup) {
            return std::unexpected(Error{120, new Error{postfixSquareBracketsGroup->error()}});
        }
        accumulatedPostfixLeftPart = new PostfixSquareBracketsGroup{variant_cast(accumulatedPostfixLeftPart), **postfixSquareBracketsGroup};
        goto BEGIN;
    }

    if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
        ProgramWordWithoutAssociation leftPart = variant_cast(accumulatedPostfixLeftPart);
        input.ignore(sequenceLen(Association::SEPARATOR_SEQUENCE)); // consume association separator characters
        if (auto rightPart = consumeProgramWord(input); !rightPart) {
            return std::unexpected(Error{121, new Error{rightPart.error()}});
        } else {
            return new Association{leftPart, *rightPart};
        }
    }

    return variant_cast(accumulatedPostfixLeftPart);
}

std::optional<MayFail<ParenthesesGroup*>> tryConsumeParenthesesGroupStrictly(std::istringstream& input) {
    if (!peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return {};
    }
    input.ignore(sequenceLen(ParenthesesGroup::INITIATOR_SEQUENCE)); // consume initiator characters

    if (input.peek() == EOF) {
        return std::unexpected(Error{122});
    }

    std::vector<Term> terms;

    if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ParenthesesGroup::TERMINATOR_SEQUENCE)); // consume terminator characters
        return new ParenthesesGroup{terms}; // empty
    }

    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(ParenthesesGroup::CONTINUATOR_SEQUENCE).value(),
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value()
    };

    if (auto term = consumeTerm(input, terminatorCharacters); !term) {
        return std::unexpected(Error{123, new Error{term.error()}});
    } else {
        terms.push_back(*term);
    }

    while (input.peek() != EOF && !peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        if (auto res = consumeSequence(ParenthesesGroup::CONTINUATOR_SEQUENCE, input); !res) {
            return std::unexpected(Error{124, new Error{res.error()}});
        }
        if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
            return std::unexpected(Error{125});
        }
        if (auto term = consumeTerm(input, terminatorCharacters); !term) {
            return std::unexpected(Error{123, new Error{term.error()}});
        } else {
            terms.push_back(*term);
        }
    }

    if (!peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Error{126});
    }
    input.ignore(sequenceLen(ParenthesesGroup::TERMINATOR_SEQUENCE)); // consume terminator characters

    return new ParenthesesGroup{terms};
}
