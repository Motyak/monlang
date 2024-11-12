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

using AssociationLeftPart_ = std::variant<
    Atom*,
    /* no MayFail_<SquareBracketsTerm>* here */
    GROUP_ENTITIES_,
    POSTFIXES_
    /* no MayFail_<Association>* here */
>;

AssociationLeftPart unwrap_assoc_left_part(AssociationLeftPart_);

///////////////////////////////////////////////////////////

struct Association {
    static const Sequence SEPARATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    AssociationLeftPart leftPart;
    Word rightPart;
};

template <>
struct MayFail_<Association> {
    AssociationLeftPart_ leftPart; // never Malformed, by design
    MayFail<Word_> rightPart;

    Association unwrap() const {
        return (Association)*this;
    }

    explicit operator Association() const {
        return Association{unwrap_assoc_left_part(leftPart), unwrap_w(rightPart.val)};
    }
};

template <typename T>
MayFail<MayFail_<Association>*> consumeAssociation(T assocLeftPart, std::istringstream& input) {
    input.ignore(sequenceLen(Association::SEPARATOR_SEQUENCE));
    auto whats_right_behind = consumeWord(input);
    auto assoc = move_to_heap(MayFail_<Association>{
        variant_cast(assocLeftPart),
        whats_right_behind
    });
    if (whats_right_behind.has_error()) {
        return Malformed(assoc, ERR(219));
    }
    return assoc;
}

#endif
