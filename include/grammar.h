#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <sstream>

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

struct Quotation {
    Quoted quoted;
};

struct ParenthesesGroup;
struct SquareBracketsGroup;
struct CurlyBracketsGroup;
struct Association;
using ProgramWord = std::variant<ParenthesesGroup*, SquareBracketsGroup*, Quotation, CurlyBracketsGroup*, Association*, Atom>;
using ProgramWordWithoutAssociation = std::variant<ParenthesesGroup*, SquareBracketsGroup*, Quotation, CurlyBracketsGroup*, Atom>;

struct ParenthesesGroup {
    std::vector<ProgramWord> words;

    static const std::vector<char> WORD_TERMINATORS;
};

struct SquareBracketsGroup {
    std::vector<ProgramWord> words;

    static const std::vector<char> WORD_TERMINATORS;
};

struct ProgramSentence {
    std::vector<ProgramWord> words;

    static const std::vector<char> WORD_TERMINATORS;
};

struct CurlyBracketsGroup {
    std::vector<ProgramSentence> sentences;

    static const std::vector<char> SENTENCE_TERMINATORS; // }
};

struct Association {
    ProgramWordWithoutAssociation leftPart;
    ProgramWord rightPart;

    static const std::vector<char> LEFT_PART_TERMINATORS; // :
    static const std::vector<char> RIGHT_PART_TERMINATORS; // SPACE or NEWLINE
};

struct Program {
    std::vector<ProgramSentence> sentences;
};

Program consumeProgram(std::istringstream&);

ProgramSentence consumeProgramSentence(const std::vector<char> terminators, std::istringstream&);

ProgramWord consumeProgramWord(const std::vector<char> terminators, std::istringstream&);
ProgramWordWithoutAssociation consumeProgramWordWithoutAssociation(const std::vector<char> terminators, std::istringstream&);

std::optional<ParenthesesGroup*> tryConsumeParenthesesGroup(const std::vector<char> terminators, std::istringstream&);
std::optional<SquareBracketsGroup*> tryConsumeSquareBracketsGroup(const std::vector<char> terminators, std::istringstream&);
std::optional<Quotation> tryConsumeQuotation(const std::vector<char> terminators, std::istringstream&);
std::optional<CurlyBracketsGroup*> tryConsumeCurlyBracketsGroup(const std::vector<char> terminators, std::istringstream&);
std::optional<Association*> tryConsumeAssociation(const std::vector<char> terminators, std::istringstream&);
Quoted consumeQuoted(const std::vector<char> terminators, std::istringstream&);
Atom consumeAtom(const std::vector<char> terminators, std::istringstream&);

void consumeSequence(std::vector<char> sequence, std::istringstream& input);

#endif // GRAMMAR_H
