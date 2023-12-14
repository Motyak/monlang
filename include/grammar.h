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

PROGRAM-WORD :=  ASSOCIATION | PARENTHESES-GROUP | SQUARE-BRACKETS-GROUP | QUOTATION | CURLY-BRACKETS-GROUP | ATOM

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
using ProgramWord = std::variant<Association*, ParenthesesGroup*, SquareBracketsGroup*, Quotation, CurlyBracketsGroup*, Atom>;
using ProgramWordWithoutAssociation = std::variant<ParenthesesGroup*, SquareBracketsGroup*, Quotation, CurlyBracketsGroup*, Atom>;

struct ParenthesesGroup {
    std::vector<ProgramWord> words;

    static const std::vector<char> CONTINUATOR_SEQUENCE;
    static const std::vector<char> ALT_CONTINUATOR_SEQUENCE;
    static const std::vector<char> TERMINATOR_SEQUENCE;
};

struct SquareBracketsGroup {
    std::vector<ProgramWord> words;

    static const std::vector<char> INITIATOR_SEQUENCE;
    static const std::vector<char> CONTINUATOR_SEQUENCE;
    static const std::vector<char> TERMINATOR_SEQUENCE;
};

struct ProgramSentence {
    std::vector<ProgramWord> words;

    static const std::vector<char> CONTINUATOR_SEQUENCE;
    static const std::vector<char> TERMINATOR_SEQUENCE;
};

struct CurlyBracketsGroup {
    std::vector<ProgramSentence> sentences;
};

struct Association {
    ProgramWordWithoutAssociation leftPart;
    ProgramWord rightPart;

    static const std::vector<char> SEPARATOR_SEQUENCE;
};

struct Program {
    std::vector<ProgramSentence> sentences;
};

Program consumeProgram(std::istringstream&);

ProgramSentence consumeProgramSentence(std::istringstream&);

ProgramWord consumeProgramWord(std::istringstream&);
ProgramWordWithoutAssociation consumeProgramWordWithoutAssociation(std::istringstream&);

enum struct AllowAssociation {
    TRUE,
    FALSE
};

std::optional<ParenthesesGroup*> tryConsumeParenthesesGroup(std::istringstream&, AllowAssociation = AllowAssociation::FALSE);
// returns either a SquareBracketsGroup or a Association between a SquareBracketsGroup and another ProgramWord (if flag enabled and association found)
std::optional<std::variant<SquareBracketsGroup*, Association*>> tryConsumeSquareBracketsGroup(std::istringstream&, AllowAssociation = AllowAssociation::FALSE);
std::optional<Quotation> tryConsumeQuotation(std::istringstream&, AllowAssociation = AllowAssociation::FALSE);
std::optional<CurlyBracketsGroup*> tryConsumeCurlyBracketsGroup(std::istringstream&, AllowAssociation = AllowAssociation::FALSE);
// std::optional<Association*> tryConsumeAssociation(std::istringstream&);
Quoted consumeQuoted(std::istringstream&, AllowAssociation = AllowAssociation::FALSE);
Atom consumeAtom(std::istringstream&, AllowAssociation = AllowAssociation::FALSE);

void consumeSequence(std::vector<char> sequence, std::istringstream&);
bool peekSequence(std::vector<char> sequence, std::istringstream&);

#endif // GRAMMAR_H
