
#include <grammar.h>

#include <sstream>
#include <vector>
#include <iostream>

#define SPACE (char(32))
#define NEWLINE (char(10))

// -------------------------------------
// struct ParenthesesGroup
// -------------------------------------

const std::vector<char> ParenthesesGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const std::vector<char> ParenthesesGroup::TERMINATOR_SEQUENCE = { ')' };

ProgramWord ParenthesesGroup::consumeProgramWord(std::istringstream& input) {
    if (auto potentialParenthesesGroup = tryConsumeParenthesesGroup(input)) {
        return *potentialParenthesesGroup;
    }
    if (auto potentialSquareBracketsGroup = tryConsumeSquareBracketsGroup(input)) {
        return *potentialSquareBracketsGroup;
    }
    if (auto potentialQuotation = tryConsumeQuotation(input)) {
        return *potentialQuotation;
    }
    if (auto potentialCurlyBracketsGroup = tryConsumeCurlyBracketsGroup(input)) {
        return *potentialCurlyBracketsGroup;
    }
    if (auto potentialAssociation = tryConsumeAssociation(input)) {
        return *potentialAssociation;
    }
    return consumeAtom(input);
}

std::optional<Association*> ParenthesesGroup::tryConsumeAssociation(std::istringstream& input) {
    
}

Atom ParenthesesGroup::consumeAtom(std::istringstream& input) {

}

// -------------------------------------
// struct ParenthesesGroup (end)
// -------------------------------------



const std::vector<char> SquareBracketsGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const std::vector<char> SquareBracketsGroup::TERMINATOR_SEQUENCE = { ']' };

const std::vector<char> ProgramSentence::CONTINUATOR_SEQUENCE = { SPACE };
const std::vector<char> ProgramSentence::TERMINATOR_SEQUENCE = { NEWLINE };

Program consumeProgram(std::istringstream& input) {
    std::vector<ProgramSentence> sentences;
    while (input) {
        auto currentSentence = consumeProgramSentence(input);
        consumeSequence(ProgramSentence::TERMINATOR_SEQUENCE, input); // throw exception if not found
        sentences.push_back(currentSentence);
    }
    return Program{sentences};
}

ProgramSentence consumeProgramSentence(std::istringstream& input) {
    std::vector<ProgramWord> words;
    auto currentWord = consumeProgramWord(input);
    words.push_back(currentWord);
    while (input && input.peek() != '\n') {
        consumeSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input);
        currentWord = consumeProgramWord(input);
        words.push_back(currentWord);
    }
    return ProgramSentence{words};
}

ProgramWord consumeProgramWord(std::istringstream& input) {
    if (auto potentialParenthesesGroup = tryConsumeParenthesesGroup(input)) {
        return *potentialParenthesesGroup;
    }
    if (auto potentialSquareBracketsGroup = tryConsumeSquareBracketsGroup(input)) {
        return *potentialSquareBracketsGroup;
    }
    if (auto potentialQuotation = tryConsumeQuotation(input)) {
        return *potentialQuotation;
    }
    if (auto potentialCurlyBracketsGroup = tryConsumeCurlyBracketsGroup(input)) {
        return *potentialCurlyBracketsGroup;
    }
    if (auto potentialAssociation = tryConsumeAssociation(input)) {
        return *potentialAssociation;
    }
    return consumeAtom(input);
}

ProgramWordWithoutAssociation consumeProgramWordWithoutAssociation(std::istringstream& input) {
    if (auto potentialParenthesesGroup = tryConsumeParenthesesGroup(input)) {
        return *potentialParenthesesGroup;
    }
    if (auto potentialSquareBracketsGroup = tryConsumeSquareBracketsGroup(input)) {
        return *potentialSquareBracketsGroup;
    }
    if (auto potentialQuotation = tryConsumeQuotation(input)) {
        return *potentialQuotation;
    }
    if (auto potentialCurlyBracketsGroup = tryConsumeCurlyBracketsGroup(input)) {
        return *potentialCurlyBracketsGroup;
    }
    return consumeAtom(input);
}

std::optional<Association*> SquareBracketsGroup::tryConsumeAssociation(std::istringstream&) {

}

Atom SquareBracketsGroup::consumeAtom(std::istringstream&) {

}

ProgramWord SquareBracketsGroup::consumeProgramWord(std::istringstream& input) {
    if (auto potentialParenthesesGroup = tryConsumeParenthesesGroup(input)) {
        return *potentialParenthesesGroup;
    }
    if (auto potentialSquareBracketsGroup = tryConsumeSquareBracketsGroup(input)) {
        return *potentialSquareBracketsGroup;
    }
    if (auto potentialQuotation = tryConsumeQuotation(input)) {
        return *potentialQuotation;
    }
    if (auto potentialCurlyBracketsGroup = tryConsumeCurlyBracketsGroup(input)) {
        return *potentialCurlyBracketsGroup;
    }
    if (auto potentialAssociation = tryConsumeAssociation(input)) {
        return *potentialAssociation;
    }
    return consumeAtom(input);
}

std::optional<SquareBracketsGroup*> tryConsumeSquareBracketsGroup(std::istringstream& input) {
    if (input.peek() != '[') {
        return {};
    }
    input.ignore(1); // consume opening square bracket

    std::vector<ProgramWord> words;

    switch (input.peek()) {
        case ']':
            input.ignore(1);
            return new SquareBracketsGroup{words};

        case EOF:
            std::cerr << "unexpected EOF while about to parse a program word in square brackets group" << std::endl;
            exit(1);
    }

    auto currentWord = SquareBracketsGroup::consumeProgramWord(input);
    words.push_back(currentWord);
    while (input && input.peek() != ']') {
        consumeSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input);
        currentWord = SquareBracketsGroup::consumeProgramWord(input);
        words.push_back(currentWord);
    }

    if (input.peek() != ']') {
        std::cerr << "was expecting `]` but found " << input.peek() << std::endl;
            exit(1);
    }
    input.ignore(1); // consume closing square bracket

    return new SquareBracketsGroup{words};
}

std::optional<Quotation> tryConsumeQuotation(std::istringstream& input) {
    
}

std::optional<CurlyBracketsGroup*> tryConsumeCurlyBracketsGroup(std::istringstream& input) {
    
}

std::optional<Association*> tryConsumeAssociation(std::istringstream& input) {
    
}

Quoted consumeQuoted(std::istringstream& input) {
    
}

Atom consumeAtom(std::istringstream& input) {
    
}

void consumeSequence(std::vector<char> sequence, std::istringstream& input) {
    for (auto c: sequence) {
        if (input.peek() != c) {
            std::cerr << "was expecting `" << c << "` but found `" << (char)input.peek() << "`" << std::endl;
            exit(1);
        }
        input.ignore(1);
    }
}
