
#include <grammar.h>

#define SPACE (char(32))
#define NEWLINE (char(10))

const std::vector<char> ParenthesesGroup::WORD_TERMINATORS = { ',', ')' };
const std::vector<char> SquareBracketsGroup::WORD_TERMINATORS = { ',', ']' };
const std::vector<char> ProgramSentence::WORD_TERMINATORS = { SPACE, NEWLINE };
const std::vector<char> CurlyBracketsGroup::SENTENCE_TERMINATORS = { NEWLINE };

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
