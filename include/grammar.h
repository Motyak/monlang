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

    static const std::vector<char> CONTINUATOR_SEQUENCE;
    static const std::vector<char> TERMINATOR_SEQUENCE;

    static ProgramWord consumeProgramWord(std::istringstream&);

  private:
    static std::optional<Association*> tryConsumeAssociation(std::istringstream&);
    static Atom consumeAtom(std::istringstream&);
};

struct SquareBracketsGroup {
    std::vector<ProgramWord> words;

    static const std::vector<char> CONTINUATOR_SEQUENCE;
    static const std::vector<char> TERMINATOR_SEQUENCE;

    static ProgramWord consumeProgramWord(std::istringstream&);

  private:
    static std::optional<Association*> tryConsumeAssociation(std::istringstream&);
    static Atom consumeAtom(std::istringstream&);
};

struct ProgramSentence {
    std::vector<ProgramWord> words;

    static const std::vector<char> CONTINUATOR_SEQUENCE;
    static const std::vector<char> TERMINATOR_SEQUENCE;

    static ProgramWord consumeProgramWord(std::istringstream&);

  private:
    static std::optional<Association*> tryConsumeAssociation(std::istringstream&);
    static Atom consumeAtom(std::istringstream&);
};

struct CurlyBracketsGroup {
    std::vector<ProgramSentence> sentences;
};

struct Association {
    ProgramWordWithoutAssociation leftPart;
    ProgramWord rightPart;
    // std::variant<ProgramSentence, ParenthesesGroup*, SquareBracketsGroup*, Association*> parent;

    static const std::vector<char> SEPARATOR_SEQUENCE;

    // std::vector<char> TERMINATOR_SEQUENCE(); // inherits terminator from parent, have to do a look-up
    static ProgramWord consumeProgramWord(std::istringstream&);
    static ProgramWordWithoutAssociation consumeProgramWordWithoutAssociation(std::istringstream&);

  private:
    static std::optional<Association*> tryConsumeAssociation(std::istringstream&);
    static Atom consumeAtom(std::istringstream&);
};

struct Program {
    std::vector<ProgramSentence> sentences;
};

Program consumeProgram(std::istringstream&);

ProgramSentence consumeProgramSentence(std::istringstream&);

// ProgramWord consumeProgramWord(std::istringstream&);
// ProgramWordWithoutAssociation consumeProgramWordWithoutAssociation(std::istringstream&);

std::optional<ParenthesesGroup*> tryConsumeParenthesesGroup(std::istringstream&);
std::optional<SquareBracketsGroup*> tryConsumeSquareBracketsGroup(std::istringstream&);
std::optional<Quotation> tryConsumeQuotation(std::istringstream&);
std::optional<CurlyBracketsGroup*> tryConsumeCurlyBracketsGroup(std::istringstream&);
// std::optional<Association*> tryConsumeAssociation(std::istringstream&);
Quoted consumeQuoted(std::istringstream&);
// Atom consumeAtom(std::istringstream&);

void consumeSequence(std::vector<char> sequence, std::istringstream& input);

#endif // GRAMMAR_H
