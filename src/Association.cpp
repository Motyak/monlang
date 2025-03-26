#include <monlang/Association.h>
#include <monlang/common.h>

/* require knowing all words for token_len() */
#include <monlang/ast/Atom.h>
#include <monlang/ast/Quotation.h>
#include <monlang/ast/SquareBracketsGroup.h>
#include <monlang/ast/CurlyBracketsGroup.h>
#include <monlang/ast/PostfixParenthesesGroup.h>
#include <monlang/ast/PostfixSquareBracketsGroup.h>
#include <monlang/ast/Path.h>

#include <utils/variant-utils.h>
#include <utils/assert-utils.h>

const Sequence Association::SEPARATOR_SEQUENCE = {':', {':', 0_}};

const std::vector<char> Association::RESERVED_CHARACTERS = {
    sequenceFirstChar(Association::SEPARATOR_SEQUENCE).value()
};

MayFail<MayFail_<Association>>
consumeAssociation(const AssociationLeftPart& assocLeftPart, std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    input.ignore(sequenceLen(Association::SEPARATOR_SEQUENCE));
    auto whats_right_behind = consumeWord(input);
    auto assoc = MayFail_<Association>{
        assocLeftPart,
        whats_right_behind
    };
    if (whats_right_behind.has_error()) {
        return Malformed(assoc, ERR(219));
    }
    assoc._tokenLen = token_len(assocLeftPart) + GET_INPUT_STREAM_PROGRESS();
    return assoc;
}

///////////////////////////////////////////////////////////

Association::Association(const AssociationLeftPart& leftPart, const Word& rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<Association>::MayFail_(const AssociationLeftPart& leftPart, const MayFail<Word_>& rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<Association>::MayFail_(const Association& assoc) {
    this->leftPart = assoc.leftPart;
    this->rightPart = wrap_w(assoc.rightPart);
    this->_tokenLen = assoc._tokenLen;
}

MayFail_<Association>::operator Association() const {
    auto assoc = Association{leftPart, unwrap_w(rightPart.value())};
    assoc._tokenLen = this->_tokenLen;
    return assoc;
}
