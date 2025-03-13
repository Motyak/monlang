#ifndef QUOTATION_H
#define QUOTATION_H

#include <monlang/ast/Quotation.h>

#include <utils/MayFail.h>

struct PostfixSquareBracketsGroup;

template <>
struct MayFail_<Quotation> {
    std::string quoted;

    size_t _tokenLen = 0;

    MayFail_() = default;
    explicit MayFail_(const std::string&);

    explicit MayFail_(Quotation);
    explicit operator Quotation() const;
};

MayFail<MayFail_<Quotation>> consumeQuotationStrictly(std::istringstream&);
MayFail<MayFail_<Quotation>> consumeMultilineQuotationStrictly(std::istringstream&);

using consumeQuotation_RetType = std::variant<
    MayFail<MayFail_<Quotation>*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>
>;
consumeQuotation_RetType consumeQuotation(std::istringstream&);
consumeQuotation_RetType consumeMultilineQuotation(std::istringstream&);

#endif // QUOTATION_H
