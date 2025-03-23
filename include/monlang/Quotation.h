#ifndef QUOTATION_H
#define QUOTATION_H

#include <monlang/ast/Quotation.h>

#include <utils/MayFail.h>

struct PostfixSquareBracketsGroup;

MayFail<Quotation> consumeQuotationStrictly(std::istringstream&);
MayFail<Quotation> consumeMultilineQuotationStrictly(std::istringstream&);

using consumeQuotation_RetType = std::variant<
    MayFail<Quotation*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>
>;
consumeQuotation_RetType consumeQuotation(std::istringstream&);
consumeQuotation_RetType consumeMultilineQuotation(std::istringstream&);

#endif // QUOTATION_H
