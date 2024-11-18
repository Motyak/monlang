#include <monlang/ast/visitors/ConvenientVisitor.h>

#include <monlang/common.h>

/* impl only */
#include <monlang/ast/SquareBracketsTerm.h>
#include <monlang/ast/ParenthesesGroup.h>
#include <monlang/ast/SquareBracketsGroup.h>
#include <monlang/ast/CurlyBracketsGroup.h>
#include <monlang/ast/PostfixParenthesesGroup.h>
#include <monlang/ast/PostfixSquareBracketsGroup.h>
#include <monlang/ast/Association.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>

void ConvenientVisitor<void>::operator()(const Program& prog) {
    for (auto progSentence: prog.sentences) {
        operator()(progSentence);
    }
}

void ConvenientVisitor<void>::operator()(const ProgramSentence& progSentence) {
    for (auto progWord: progSentence.programWords) {
        operator()(progWord);
    }
}

void ConvenientVisitor<void>::operator()(const ProgramWord& progWord){
    std::visit(*this, progWord);
}

void ConvenientVisitor<void>::operator()(const Term& term) {
    for (auto word: term.words) {
        operator()(word);
    }
}

void ConvenientVisitor<void>::operator()(const Word& word) {
    std::visit(*this, word);
}

////////////////////////////////////////////////////////////////

void ConvenientVisitor<void>::operator()(Atom*){} // operation-specific

void ConvenientVisitor<void>::operator()(SquareBracketsTerm* sbt) {
    operator()(sbt->term);
}

void ConvenientVisitor<void>::operator()(ParenthesesGroup* pg) {
    for (auto term: pg->terms) {
        operator()(term);
    }
}

void ConvenientVisitor<void>::operator()(SquareBracketsGroup* sbg) {
    for (auto term: sbg->terms) {
        operator()(term);
    }
}

void ConvenientVisitor<void>::operator()(CurlyBracketsGroup* cbg) {
    for (auto sentence: cbg->sentences) {
        operator()(sentence);
    }
}

void ConvenientVisitor<void>::operator()(PostfixParenthesesGroup* ppg) {
    operator()(ppg->leftPart);
    operator()(move_to_heap(ppg->rightPart));
}

void ConvenientVisitor<void>::operator()(PostfixSquareBracketsGroup* psbg) {
    operator()(psbg->leftPart);
    operator()(move_to_heap(psbg->rightPart));
}

void ConvenientVisitor<void>::operator()(Association* assoc) {
    operator()((Word)variant_cast(assoc->leftPart));
    operator()(assoc->rightPart);
}
