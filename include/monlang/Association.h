#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include <monlang/ast/Association.h>

#include <monlang/Word.h>

template <>
struct MayFail_<Association> {
    AssociationLeftPart leftPart; // never Malformed, by design
    MayFail<Word_> rightPart;

    size_t _tokenLen = 0;
    explicit MayFail_(const AssociationLeftPart&, const MayFail<Word_>&);

    explicit MayFail_(const Association&);
    explicit operator Association() const;
};

template <typename T>
MayFail<MayFail_<Association>*> consumeAssociation(const T& assocLeftPart, std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    input.ignore(sequenceLen(Association::SEPARATOR_SEQUENCE));
    auto whats_right_behind = consumeWord(input);
    auto assoc = move_to_heap(MayFail_<Association>{
        variant_cast(assocLeftPart),
        whats_right_behind
    });
    if (whats_right_behind.has_error()) {
        return Malformed(assoc, ERR(219));
    }
    assoc->_tokenLen = token_len(assocLeftPart) + GET_INPUT_STREAM_PROGRESS();
    return assoc;
}

#endif // ASSOCIATION_H
