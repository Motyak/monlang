#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include <monlang/Word.h>
#include <monlang/common.h>

#include <variant>

using AssociationLeftPart = std::variant<
    Atom*,
    /* no SquareBracketsTerm* here */
    GROUP_ENTITIES,
    POSTFIXES
    /* no Association* here */
>;

template <>
MayFail<AssociationLeftPart> mayfail_cast<AssociationLeftPart>(MayFail<Word> inputMayfail);

///////////////////////////////////////////////////////////

struct Association {
    static const Sequence SEPARATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    AssociationLeftPart leftPart; // never Malformed, by design
    MayFail<Word> rightPart;
};

template <typename T>
std::optional<MayFail<Association*>>
tryConsumeAssociation(T assocLeftPart, std::istringstream& input) {
    if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(Association::SEPARATOR_SEQUENCE));
        auto whats_right_behind = consumeWord(input);
        auto assoc = move_to_heap(Association{
            variant_cast(assocLeftPart),
            whats_right_behind
        });
        if (!whats_right_behind.has_value()) {
            return std::unexpected(Malformed(assoc, ERR(219)));
        }
        return assoc;
    }
    return {}; // nothing found
}

#endif
