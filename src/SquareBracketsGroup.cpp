#include <monlang/SquareBracketsGroup.h>
#include <monlang/Term.h>
#include <monlang/common.h>

/* impl only */
#include <monlang/PostfixSquareBracketsGroup.h>

#include <algorithm>

#define until(x) while(!(x))

const Sequence SquareBracketsGroup::INITIATOR_SEQUENCE = {'['};
const Sequence SquareBracketsGroup::CONTINUATOR_SEQUENCE = {',', SPACE};
const Sequence SquareBracketsGroup::TERMINATOR_SEQUENCE = {']'};

const std::vector<char> SquareBracketsGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<SquareBracketsGroup> consumeSquareBracketsGroupStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(SquareBracketsGroup::TERMINATOR_SEQUENCE).value()
    };

    if (!consumeSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(SquareBracketsGroup{}, Error{43}));
    }
    if (peekSequence(SquareBracketsGroup::CONTINUATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(SquareBracketsGroup{}, Error{431}));
    }
    if (peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(SquareBracketsGroup::TERMINATOR_SEQUENCE));
        return SquareBracketsGroup{};
    }

    std::vector<MayFail<Term>> terms;
    MayFail<Term> currentTerm;

    std::vector<char> termTerminatorChars = {
        sequenceFirstChar(SquareBracketsGroup::CONTINUATOR_SEQUENCE).value(),
        sequenceFirstChar(SquareBracketsGroup::TERMINATOR_SEQUENCE).value()
    };
    currentTerm = consumeTerm(termTerminatorChars, input);
    terms.push_back(currentTerm);
    if (!currentTerm.has_value()) {
        return std::unexpected(Malformed(SquareBracketsGroup{terms}, Error{439}));
    }

    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        
        if (!consumeSequence(SquareBracketsGroup::CONTINUATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(SquareBracketsGroup{terms}, Error{403}));
        }
        // if (peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        //     return std::unexpected(Malformed(SquareBracketsGroup{terms}, Error{432}));
        // }

        currentTerm = consumeTerm(termTerminatorChars, input);
        terms.push_back(currentTerm);
        if (!currentTerm.has_value()) {
            return std::unexpected(Malformed(SquareBracketsGroup{terms}, Error{439}));
        }
    }

    if (!consumeSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(SquareBracketsGroup{terms}, Error{430}));
    }

    return SquareBracketsGroup{terms};
}

consumeSquareBracketsGroup_RetType consumeSquareBracketsGroup(std::istringstream& input) {
    return mayfail_convert<SquareBracketsGroup*>(consumeSquareBracketsGroupStrictly(input)); // TODO: TMP IMPL
}
