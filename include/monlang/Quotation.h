#ifndef QUOTATION_H
#define QUOTATION_H

#include <monlang/ast/Quotation.h>

#include <utils/MayFail.h>

MayFail<Quotation> consumeQuotation(std::istringstream&);
MayFail<Quotation> consumeMultilineQuotation(std::istringstream&);

#endif // QUOTATION_H
