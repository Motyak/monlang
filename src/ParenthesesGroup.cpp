#include <monlang/ParenthesesGroup.h>
#include <monlang/Term.h>
#include <monlang/common.h>

/* impl only */
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/loop-utils.h>
#include <utils/variant-utils.h>

#include <algorithm>

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
    const std::vector<char> termTerminatorChars = {
        sequenceFirstChar(ParenthesesGroup::CONTINUATOR_SEQUENCE).value(),
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value()
    };

    if (!consumeSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ParenthesesGroup{}, ERR(042)));
    }

    if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ParenthesesGroup::TERMINATOR_SEQUENCE));
        return ParenthesesGroup{};
    }

    std::vector<MayFail<Term>> terms;
    MayFail<Term> currentTerm;

    currentTerm = consumeTerm(termTerminatorChars, input);
    terms.push_back(currentTerm);
    if (!currentTerm.has_value()) {
        return std::unexpected(Malformed(ParenthesesGroup{terms}, ERR(429)));
    }

    LOOP until (input.peek() == EOF || peekAnyChar(terminatorCharacters, input)) {
    if (!__first_it)
    {
        if (!consumeSequence(ParenthesesGroup::CONTINUATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(ParenthesesGroup{terms}, ERR(402)));
        }
    }
        currentTerm = consumeTerm(termTerminatorChars, input);
        terms.push_back(currentTerm);
        if (!currentTerm.has_value()) {
            return std::unexpected(Malformed(ParenthesesGroup{terms}, ERR(429)));
        }

        ENDLOOP
    }

    if (!consumeSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ParenthesesGroup{terms}, ERR(420)));
    }

    return ParenthesesGroup{terms};
}

consumeParenthesesGroup_RetType consumeParenthesesGroup(std::istringstream& input) {
    auto pg = consumeParenthesesGroupStrictly(input);

    if (!pg.has_value()) {
        return mayfail_convert<ParenthesesGroup*>(pg);
    }

    using PostfixLeftPart = std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap(pg.value());

    [[maybe_unused]]
    BEGIN:
    #ifndef DISABLE_PPG_IN_ATOM
    if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        auto whats_right_behind = consumeParenthesesGroupStrictly(input);
        auto curr_ppg = move_to_heap(PostfixParenthesesGroup{
            variant_cast(accumulatedPostfixLeftPart),
            whats_right_behind
        });
        if (!whats_right_behind.has_value()) {
            return std::unexpected(Malformed(curr_ppg, ERR(319)));
        }
        accumulatedPostfixLeftPart = curr_ppg;
        goto BEGIN;
    }
    #endif

    #ifndef DISABLE_PSBG_IN_ATOM
    if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        auto whats_right_behind = consumeSquareBracketsGroupStrictly(input);
        auto curr_psbg = move_to_heap(PostfixSquareBracketsGroup{
            variant_cast(accumulatedPostfixLeftPart),
            whats_right_behind
        });
        if (!whats_right_behind.has_value()) {
            return std::unexpected(Malformed(curr_psbg, ERR(329)));
        }
        accumulatedPostfixLeftPart = curr_psbg;
        goto BEGIN;
    }
    #endif

    return std::visit(
        [](auto word) -> consumeParenthesesGroup_RetType {return word;},
        accumulatedPostfixLeftPart
    );
}
