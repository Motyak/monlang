#include <CurlyBracketsGroup.h>
#include <ProgramSentence.h>
#include <ParenthesesGroup.h>
#include <SquareBracketsGroup.h>
#include <PostfixParenthesesGroup.h>
#include <PostfixSquareBracketsGroup.h>
#include <Association.h>
#include <common.h>
#include <utils/assert-utils.h>
#include <utils/str-utils.h>
#include <utils/variant-utils.h>

#include <iostream>

const std::vector<CharacterAppearance> CurlyBracketsGroup::INITIATOR_SEQUENCE = { '{' };
const std::vector<CharacterAppearance> CurlyBracketsGroup::TERMINATOR_SEQUENCE = { '}' };

const std::vector<char> CurlyBracketsGroup::RESERVED_CHARACTERS = {
    INITIATOR_SEQUENCE[0],
    TERMINATOR_SEQUENCE[0]
};

std::optional<std::variant<CurlyBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> tryConsumeCurlyBracketsGroup(std::istringstream& input) {
    auto curlyBracketsGroup = tryConsumeCurlyBracketsGroupStrictly(input);
    if (!curlyBracketsGroup) {
        return {};
    }

    using PostfixLeftPart = std::variant<CurlyBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = *curlyBracketsGroup;

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

static CurlyBracketsGroup* consumeOnelineGroup(std::istringstream&);

std::optional<CurlyBracketsGroup*> tryConsumeCurlyBracketsGroupStrictly(std::istringstream& input) {
    if (!peekSequence(CurlyBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return {};
    }
    input.ignore(CurlyBracketsGroup::INITIATOR_SEQUENCE.size()); // consume initiator characters

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while entering a curly brackets group" << std::endl;
        throw std::runtime_error("user exception");
    }

    std::vector<ProgramSentence> sentences;

    if (peekSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(CurlyBracketsGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters
        return new CurlyBracketsGroup{sentences}; // empty
    }

    if (input.peek() != NEWLINE) {
        return consumeOnelineGroup(input);
    }

    input.ignore(1); // consume NEWLINE

    ProgramSentence currentSentence;
    try {
        currentSentence = consumeProgramSentence(input);
    } catch (std::runtime_error& e) {
        std::cerr << "expected a program sentence" << std::endl;
        throw std::runtime_error("user exception");
    }
    sentences.push_back(currentSentence);
    while (input.peek() != EOF && !peekSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        try {
            currentSentence = consumeProgramSentence(input);
        } catch (std::runtime_error& e) {
            std::cerr << "expected a program sentence" << std::endl;
            throw std::runtime_error("user exception");
        }
        sentences.push_back(currentSentence);
    }

    if (!peekSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        auto& ts = CurlyBracketsGroup::TERMINATOR_SEQUENCE;
        std::cerr << "was expecting " << str(ts)
                << " but found " << str(input.peek()) << std::endl;
        throw std::runtime_error("user exception");
        
    }
    input.ignore(CurlyBracketsGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters

    return new CurlyBracketsGroup{sentences};
}

CurlyBracketsGroup* consumeOnelineGroup(std::istringstream& input) {
    std::vector<char> terminatorCharacters = {
        firstChar(CurlyBracketsGroup::TERMINATOR_SEQUENCE)
    };
    Term term;
    try {
        term = consumeTerm(input, terminatorCharacters);
    } catch (std::runtime_error& e) {
        std::cerr << "was expecting end of curly brackets group" 
                << " but got " << str(input.peek()) << std::endl;
        throw std::runtime_error("user exception");
    }

    if (!peekSequence(CurlyBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        auto& ts = CurlyBracketsGroup::TERMINATOR_SEQUENCE;
        std::cerr << "was expecting " << str(ts)
                << " but got " << str(input.peek()) << std::endl;
        throw std::runtime_error("user exception");
    }

    input.ignore(1); // consume end of curly brackets group characters sequence

    return new CurlyBracketsGroup{{ProgramSentence{term}}};
}
