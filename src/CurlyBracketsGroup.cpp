#include <CurlyBracketsGroup.h>
#include <ParenthesesGroup.h>
#include <SquareBracketsGroup.h>
#include <PostfixParenthesesGroup.h>
#include <PostfixSquareBracketsGroup.h>
#include <Association.h>
#include <utils/str-utils.h>
#include <utils/variant-utils.h>

#include <iostream>

const std::vector<CharacterAppearance> CurlyBracketsGroup::INITIATOR_SEQUENCE = { '{', {NEWLINE, 0} };
const std::vector<CharacterAppearance> CurlyBracketsGroup::TERMINATOR_SEQUENCE = { '}', {NEWLINE, 0} };

const std::vector<CharacterAppearance> CurlyBracketsGroup::ALT_INITIATOR_SEQUENCE = { '{', NEWLINE, TABS_PLUS_1 };
const std::vector<CharacterAppearance> CurlyBracketsGroup::CONTINUATOR_SEQUENCE = { NEWLINE, TABS_PLUS_1 };
const std::vector<CharacterAppearance> CurlyBracketsGroup::ALT_TERMINATOR_SEQUENCE = { NEWLINE, TABS, '}' };

const std::vector<char> CurlyBracketsGroup::RESERVED_CHARACTERS = {
    firstChar(INITIATOR_SEQUENCE),
    firstChar(TERMINATOR_SEQUENCE),
    firstChar(ALT_INITIATOR_SEQUENCE),
    firstChar(CONTINUATOR_SEQUENCE),
    firstChar(ALT_TERMINATOR_SEQUENCE)
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
        input.ignore(sequenceLen(Association::SEPARATOR_SEQUENCE)); // consume association separator characters
        ProgramWord rightPart = consumeProgramWord(input);
        return new Association{leftPart, rightPart};
    }

    return variant_cast(accumulatedPostfixLeftPart);
}

static CurlyBracketsGroup* consumeOnelineGroup(std::istringstream&);
static CurlyBracketsGroup* consumeMultilineGroup(std::istringstream&);

std::optional<CurlyBracketsGroup*> tryConsumeCurlyBracketsGroupStrictly(std::istringstream& input) {
    if (peekSequence(CurlyBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return consumeOnelineGroup(input);
    }

    if (peekSequence(CurlyBracketsGroup::ALT_INITIATOR_SEQUENCE, input)) {
        return consumeMultilineGroup(input);
    }

    return {};
}

CurlyBracketsGroup* consumeOnelineGroup(std::istringstream& input) {
    input.ignore(sequenceLen(CurlyBracketsGroup::INITIATOR_SEQUENCE)); // consume initiator characters

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while entering a oneline curly brackets group" << std::endl;
        throw std::runtime_error("user exception");
    }

    std::vector<char> terminatorCharacters = {
        firstChar(CurlyBracketsGroup::TERMINATOR_SEQUENCE)
    };
    ProgramSentence sentence;
    try {
        sentence = consumeProgramSentence(input, terminatorCharacters);
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

    return new CurlyBracketsGroup{{sentence}}; // 1-length vector
}

CurlyBracketsGroup* consumeMultilineGroup(std::istringstream& input) {
    input.ignore(sequenceLen(CurlyBracketsGroup::ALT_INITIATOR_SEQUENCE)); // consume initiator characters
    g_currentNestedLevel++; // add one nesting level

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while entering a multiline curly brackets group" << std::endl;
        throw std::runtime_error("user exception");
    }

    std::vector<ProgramSentence> sentences;

    if (peekSequence(CurlyBracketsGroup::ALT_TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(CurlyBracketsGroup::ALT_TERMINATOR_SEQUENCE)); // consume terminator characters
        return new CurlyBracketsGroup{sentences}; // empty
    }

    std::vector<char> terminatorCharacters = {
        firstChar(CurlyBracketsGroup::CONTINUATOR_SEQUENCE),
        firstChar(CurlyBracketsGroup::ALT_TERMINATOR_SEQUENCE)
    };
    ProgramSentence currentSentence;
    try {
        currentSentence = consumeProgramSentence(input, terminatorCharacters);
    } catch (std::runtime_error& e) {
        std::cerr << "was expecting end of curly brackets group" 
                << " but got " << str(input.peek()) << std::endl;
        throw new std::runtime_error("user exception");
    }
    sentences.push_back(currentSentence);
    while (input.peek() != EOF && !peekSequence(CurlyBracketsGroup::ALT_TERMINATOR_SEQUENCE, input)) {
        consumeSequence(CurlyBracketsGroup::CONTINUATOR_SEQUENCE, input);
        if (peekSequence(CurlyBracketsGroup::ALT_TERMINATOR_SEQUENCE, input)) {
            std::cerr << "was expecting another sentence" << std::endl;
            throw std::runtime_error("user exception");
        }
        try {
            currentSentence = consumeProgramSentence(input, terminatorCharacters);
        } catch (std::runtime_error& e) {
            std::cerr << "was expecting end of curly brackets group" << std::endl;
            throw new std::runtime_error("user exception");
        }
        sentences.push_back(currentSentence);
    }

    if (!peekSequence(CurlyBracketsGroup::ALT_TERMINATOR_SEQUENCE, input)) {
        auto& ts = CurlyBracketsGroup::ALT_TERMINATOR_SEQUENCE;
        std::cerr << "was expecting " << str(ts)
                << " but got " << str(input.peek()) << std::endl;
        throw std::runtime_error("user exception");
        
    }

    input.ignore(sequenceLen(CurlyBracketsGroup::ALT_TERMINATOR_SEQUENCE)); // consume terminator characters
    g_currentNestedLevel--; // remove one nested level

    return new CurlyBracketsGroup{sentences};
}
