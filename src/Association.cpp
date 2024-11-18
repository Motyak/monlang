#include <monlang/Association.h>
#include <monlang/common.h>

#include <utils/variant-utils.h>
#include <utils/assert-utils.h>

const Sequence Association::SEPARATOR_SEQUENCE = {':'};

const std::vector<char> Association::RESERVED_CHARACTERS = {
    sequenceFirstChar(Association::SEPARATOR_SEQUENCE).value()
};

///////////////////////////////////////////////////////////

MayFail_<Association>::MayFail_(AssociationLeftPart leftPart, MayFail<Word_> rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<Association>::MayFail_(Association assoc) : MayFail_(wrap(assoc)){}

MayFail_<Association>::operator Association() const {
    return Association{leftPart, unwrap_w(rightPart.value())};
}

template <>
Association unwrap(const MayFail_<Association>& assoc) {
    return (Association)assoc;
}

template <>
MayFail_<Association> wrap(const Association& assoc) {
    return MayFail_<Association>{assoc.leftPart, wrap_w(assoc.rightPart)};
}
