#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include <monlang/ast/Association.h>

#include <monlang/Word.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>

template <>
struct MayFail_<Association> {
    AssociationLeftPart leftPart; // never Malformed, by design
    MayFail<Word_> rightPart;

    size_t _tokenLen = 0;
    size_t _tokenId = 123456789;
    explicit MayFail_(const AssociationLeftPart&, const MayFail<Word_>&);

    explicit MayFail_(const Association&);
    explicit operator Association() const;
};

MayFail<MayFail_<Association>>
consumeAssociation(const AssociationLeftPart&, std::istringstream&);

#endif // ASSOCIATION_H
