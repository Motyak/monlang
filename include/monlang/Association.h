#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include <monlang/ast/Association.h>

#include <monlang/Word.h>

template <>
struct MayFail_<Association> {
    AssociationLeftPart leftPart; // never Malformed, by design
    MayFail<Word_> rightPart;

    explicit MayFail_(AssociationLeftPart, MayFail<Word_>);

    explicit MayFail_(Association);
    explicit operator Association() const;
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

#endif // ASSOCIATION_H
