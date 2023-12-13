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

PROGRAM-WORD := ASSOCIATION | PARENTHESES-GROUP | SQUARE-BRACKETS-GROUP | DOUBLE-QUOTES-GROUP | CURLY-BRACKETS-GROUP | ATOM

---

ASSOCIATION := {PROGRAM-WORD - ASSOCIATION} ':' PROGRAM-WORD

PARENTHESES-GROUP := '(' (PROGRAM-WORD 
    (',' SPACE PROGRAM-WORD)* 
    | (SPACE PROGRAM-WORD)*
    )? ')'

SQUARE-BRACKETS-GROUP := '[' (PROGRAM-WORD (',' SPACE PROGRAM-WORD)*)? ']'

DOUBLE-QUOTES-GROUP := '"' ATOM '"'
// note: should be /(?!\)"/ rather than /"/, otherwise escaped double quote can be misinterpreted

CURLY-BRACKETS-GROUP := '{' PROGRAM-SENTENCE? | (NEWLINE PROGRAM-SENTENCE (NEWLINE PROGRAM-SENTENCE)*) '}'
*/

struct Atom {
    std::string value;
};

struct ParenthesesGroup {
    std::vector<Atom> atoms;
};

struct SquareBracketsGroup {
    std::vector<Atom> atoms;
};

struct DoubleQuotesGroup {
    Atom atom;
};

struct ProgramSentence;
struct CurlyBracketsGroup {
    std::vector<ProgramSentence> sentences;
};

struct Association;
using ProgramWord = std::variant<Association, ParenthesesGroup, SquareBracketsGroup, DoubleQuotesGroup, CurlyBracketsGroup, Atom>;

using ProgramWordWithoutAssociation = std::variant<ParenthesesGroup, SquareBracketsGroup, DoubleQuotesGroup, CurlyBracketsGroup, Atom>;

struct Association {
    ProgramWordWithoutAssociation firstWord;
    ProgramWord secondWord;
};

struct ProgramSentence {
    std::vector<ProgramWord> words;
};

struct Program {
    std::vector<ProgramSentence> sentences;
};

#endif // GRAMMAR_STRUCTS_H
