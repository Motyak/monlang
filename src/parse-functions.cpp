#include <parse-functions.h>
#include <variant-utils.h>

#include <iostream>

Program consumeProgram(std::istringstream& input) {
    std::vector<ProgramSentence*> sentences;
    while (input) {
        auto currentSentence = consumeProgramSentence(input);
        consumeSequence({'\n'}, input); // throw exception if not found
        sentences.push_back(currentSentence);
    }
    return Program{sentences};
}

ProgramSentence* consumeProgramSentence(std::istringstream& input) {
    std::vector<ProgramWord> words;
    auto currentWord = consumeProgramWord(input);
    words.push_back(currentWord);
    while (input && input.peek() != '\n') {
        consumeSequence({' '}, input);
        currentWord = consumeProgramWord(input);
        words.push_back(currentWord);
    }
    return new ProgramSentence{words};
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

std::optional<ParenthesesGroup> tryConsumeParenthesesGroup(std::istringstream& input) {
    // backup initial istringstream, try stuff, then if it fails rollback (restore backup)
    // save initial position
    auto initialPosition = input.tellg();

    // restore initial position
    input.seekg(initialPosition);
}

std::optional<SquareBracketsGroup> tryConsumeSquareBracketsGroup(std::istringstream& input) {
    if (input.peek() != '[') {
        return {};
    }
    input.ignore(1); // consume opening square bracket

    std::vector<ProgramWord> words;

    switch (input.peek()) {
        case ']':
            input.ignore(1);
            return SquareBracketsGroup{words};

        case EOF:
            std::cerr << "unexpected EOF while about to parse a program word in square brackets group" << std::endl;
            exit(1);
    }

    auto currentWord = consumeProgramWord(input);
    words.push_back(currentWord);
    while (input && input.peek() != ']') {
        consumeSequence({',', ' '}, input);
        currentWord = consumeProgramWord(input);
        words.push_back(currentWord);
    }

    if (input.peek() != ']') {
        std::cerr << "was expecting `]` but found " << input.peek() << std::endl;
            exit(1);
    }
    input.ignore(1); // consume closing square bracket

    return SquareBracketsGroup{words};
}

std::optional<Quotation> tryConsumeQuotation(std::istringstream& input) {
    if (input.peek() != '"') {
        return {};
    }

    input.ignore(1); // consume opening quote

    auto quoted = consumeQuoted(input);

    if (input.peek() != '"') {
        std::cerr << "was expecting `\"` but found " << input.peek() << std::endl;
            exit(1);
    }

    input.ignore(1); // consume closing quote

    return Quotation{quoted};
}

std::optional<CurlyBracketsGroup> tryConsumeCurlyBracketsGroup(std::istringstream& input) {

}

std::optional<Association*> tryConsumeAssociation(std::istringstream& input) {
    // std::optional<Association*> res;
    // auto i = word.find(':');
    // if (i != std::string::npos) {
    //     std::string firstWordStr = word.substr(0, i - 1);
    //     std::string secondWordStr = word.substr(i + 1);
    //     ProgramWordWithoutAssociation firstWord = parseProgramWordWithoutAssociation(firstWordStr);
    //     ProgramWord secondWord = parseProgramWord(secondWordStr);
    //     res = new Association{firstWord, secondWord};
    // }
    // return res;
}

Quoted consumeQuoted(std::istringstream& input) {
    char* quoted = nullptr;
    input.get(quoted, 1000, '"'); // 1000 characters as max quoted length
    return Quoted{std::string(quoted)};
}

Atom consumeAtom(std::istringstream& input) {
    char* atom = nullptr;
    input.get(atom, 1000, '\n'); // 1000 characters as max atom length
    return Atom{std::string(atom)};
}

void consumeSequence(std::vector<char> sequence, std::istringstream& input) {
    for (auto c: sequence) {
        if (input.peek() != c) {
            std::cerr << "was expecting `" << c << "` but found " << input.peek() << std::endl;
            exit(1);
        }
        input.ignore(1);
    }
}
