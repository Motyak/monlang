#include <monlang/ParenthesesGroup.h>
#include <monlang/Term.h>
#include <monlang/common.h>

/* impl only */
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <algorithm>

#define until(x) while(!(x))

const Sequence ParenthesesGroup::INITIATOR_SEQUENCE = {'('};
const Sequence ParenthesesGroup::CONTINUATOR_SEQUENCE = {',', SPACE};
const Sequence ParenthesesGroup::TERMINATOR_SEQUENCE = {')'};

const std::vector<char> ParenthesesGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<ParenthesesGroup> consumeParenthesesGroupStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value()
    };

    if (!consumeSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ParenthesesGroup{}, Error{42}));
    }
    if (peekSequence(ParenthesesGroup::CONTINUATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ParenthesesGroup{}, Error{421}));
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
        return std::unexpected(Malformed(ParenthesesGroup{terms}, Error{429}));
    }

    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        
        if (!consumeSequence(ParenthesesGroup::CONTINUATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(ParenthesesGroup{terms}, Error{402}));
        }
        // if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        //     return std::unexpected(Malformed(ParenthesesGroup{terms}, Error{422}));
        // }

        currentTerm = consumeTerm(termTerminatorChars, input);
        terms.push_back(currentTerm);
        if (!currentTerm.has_value()) {
            return std::unexpected(Malformed(ParenthesesGroup{terms}, Error{429}));
        }
    }

    if (!consumeSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ParenthesesGroup{terms}, Error{420}));
    }

    return ParenthesesGroup{terms};
}

consumeParenthesesGroup_RetType consumeParenthesesGroup(std::istringstream& input) {
    return mayfail_convert<ParenthesesGroup*>(consumeParenthesesGroupStrictly(input)); // TODO: TMP IMPL
}
