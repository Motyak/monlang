#include <monlang/Association.h>
#include <monlang/common.h>

#include <utils/variant-utils.h>

const Sequence Association::SEPARATOR_SEQUENCE = {':'};

const std::vector<char> Association::RESERVED_CHARACTERS = {
    sequenceFirstChar(Association::SEPARATOR_SEQUENCE).value()
};

static AssociationLeftPart w2alp(Word w) {
    return std::visit(overload{
        [](Association*) -> AssociationLeftPart {SHOULD_NOT_HAPPEN();},
        [](auto word) -> AssociationLeftPart {return word;},
    }, w);
}

template <>
MayFail<AssociationLeftPart> mayfail_cast<AssociationLeftPart>(MayFail<Word> inputMayfail) {
    return inputMayfail.transform([](auto t) -> AssociationLeftPart {return w2alp(t);})
        .transform_error([](auto e) -> Malformed<AssociationLeftPart> {return {w2alp(e.val), e.err};});
}
