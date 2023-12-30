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

// std::optional<std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> tryConsumeParenthesesGroup(std::istringstream& input) {
//     auto parenthesesGroup = tryConsumeParenthesesGroupStrictly(input);
//     if (!parenthesesGroup) {
//         return {};
//     }

//     using PostfixLeftPart = std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
//     PostfixLeftPart accumulatedPostfixLeftPart = *parenthesesGroup;

//     BEGIN:
//     if (auto postfixParenthesesGroup = tryConsumeParenthesesGroupStrictly(input)) {
//         accumulatedPostfixLeftPart = new PostfixParenthesesGroup{variant_cast(accumulatedPostfixLeftPart), *postfixParenthesesGroup};
//         goto BEGIN;
//     }

//     if (auto postfixSquareBracketsGroup = tryConsumeSquareBracketsGroupStrictly(input)) {
//         accumulatedPostfixLeftPart = new PostfixSquareBracketsGroup{variant_cast(accumulatedPostfixLeftPart), *postfixSquareBracketsGroup};
//         goto BEGIN;
//     }

//     if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
//         ProgramWordWithoutAssociation leftPart = variant_cast(accumulatedPostfixLeftPart);
//         input.ignore(sequenceLen(Association::SEPARATOR_SEQUENCE)); // consume association separator characters
//         ProgramWord rightPart = consumeProgramWord(input);
//         return new Association{leftPart, rightPart};
//     }

//     return variant_cast(accumulatedPostfixLeftPart);
// }

// std::optional<ParenthesesGroup*> tryConsumeParenthesesGroupStrictly(std::istringstream& input) {
//     if (!peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
//         return {};
//     }
//     input.ignore(sequenceLen(ParenthesesGroup::INITIATOR_SEQUENCE)); // consume initiator characters

//     if (input.peek() == EOF) {
//         std::cerr << "unexpected EOF while entering a square brackets group" << std::endl;
//         throw std::runtime_error("user exception");
//     }

//     std::vector<Term> terms;

//     if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
//         input.ignore(sequenceLen(ParenthesesGroup::TERMINATOR_SEQUENCE)); // consume terminator characters
//         return new ParenthesesGroup{terms}; // empty
//     }

//     std::vector<char> terminatorCharacters = {
//         firstChar(ParenthesesGroup::CONTINUATOR_SEQUENCE),
//         firstChar(ParenthesesGroup::TERMINATOR_SEQUENCE)
//     };
//     auto currentTerm = consumeTerm(input, terminatorCharacters);
//     terms.push_back(currentTerm);
//     while (input.peek() != EOF && !peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
//         consumeSequence(ParenthesesGroup::CONTINUATOR_SEQUENCE, input);
//         currentTerm = consumeTerm(input, terminatorCharacters);
//         terms.push_back(currentTerm);
//     }

//     if (!peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
//         auto& ts = ParenthesesGroup::TERMINATOR_SEQUENCE;
//         std::cerr << "was expecting " << str(ts)
//                 << " but got " << str(input.peek()) << std::endl;
//         throw std::runtime_error("user exception");
//     }
//     input.ignore(sequenceLen(ParenthesesGroup::TERMINATOR_SEQUENCE)); // consume terminator characters

//     return new ParenthesesGroup{terms};
// }
