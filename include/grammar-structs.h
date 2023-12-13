#ifndef GRAMMAR_STRUCTS_H
#define GRAMMAR_STRUCTS_H

#include <string>
#include <vector>
#include <variant>

/*
PROGRAM := (PROGRAM-SENTENCE NEWLINE)*

---

PROGRAM-SENTENCE := PROGRAM-WORD (SPACE PROG-WORD)*

---

PROGRAM-WORD :=  PARENTHESES-GROUP | SQUARE-BRACKETS-GROUP | QUOTATION | CURLY-BRACKETS-GROUP | ASSOCIATION | ATOM

---

ASSOCIATION := {PROGRAM-WORD - ASSOCIATION} ':' PROGRAM-WORD

PARENTHESES-GROUP := '(' (PROGRAM-WORD 
    (',' SPACE PROGRAM-WORD)* 
    | (SPACE PROGRAM-WORD)*
    )? ')'

SQUARE-BRACKETS-GROUP := '[' (PROGRAM-WORD (',' SPACE PROGRAM-WORD)*)? ']'

QUOTATION := '"' QUOTED '"'
// note: should be /(?!\)"/ rather than /"/, otherwise escaped double quote can be misinterpreted

CURLY-BRACKETS-GROUP := '{' PROGRAM-SENTENCE? | (NEWLINE PROGRAM-SENTENCE (NEWLINE PROGRAM-SENTENCE)*) '}'
*/

struct Atom {
    std::string value;
};

struct Quoted {
    std::string value;
};

struct ParenthesesGroup {
    std::vector<ProgramWord> words;
};

struct SquareBracketsGroup {
    std::vector<ProgramWord> words;
};

struct Quotation {
    Quoted quoted;
};

struct ProgramSentence;
struct CurlyBracketsGroup {
    std::vector<ProgramSentence*> sentences;
};

struct Association;
using ProgramWord = std::variant<Association*, ParenthesesGroup, SquareBracketsGroup, Quotation, CurlyBracketsGroup, Atom>;

using ProgramWordWithoutAssociation = std::variant<ParenthesesGroup, SquareBracketsGroup, Quotation, CurlyBracketsGroup, Atom>;

struct Association {
    ProgramWordWithoutAssociation firstWord;
    ProgramWord secondWord;
};

struct ProgramSentence {
    std::vector<ProgramWord> words;
};

struct Program {
    std::vector<ProgramSentence*> sentences;
};

#endif // GRAMMAR_STRUCTS_H
