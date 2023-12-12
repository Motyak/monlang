#include <parse-functions.h>
#include <variant-utils.h>

#include <iostream>
#include <sstream>
#include <cstring>

Program parseProgram(const std::string& input) {
    std::vector<ProgramSentence*> res;

    // for (auto& sentence: inputLines) {
    //     /* remove comments and extra whitespaces */
    //     // TODO: 

    //     std::cout << "DEBUG sentence: ```" << sentence << "```" << std::endl;
    //     res.push_back(parseProgramSentence(sentence));
    // }

    return Program{res};
}

ProgramSentence* parseProgramSentence(const std::string& sentence) {
    std::vector<ProgramWord> res;

    std::vector<std::string> words;
    std::string currentWord;

    while (std::getline(std::istringstream(sentence), currentWord, ' ')) {
        words.push_back(currentWord);
    }

    for (auto& word: words) {
        /* remove empty words */
        // TODO: 

        std::cout << "DEBUG word: ```" << word << "```" << std::endl;
        res.push_back(parseProgramWord(word));
    }

    return new ProgramSentence{res};
}

ProgramWord parseProgramWord(const std::string& word) {
    if (auto potentialAssociation = tryParseAssociation(word)) {
        return *potentialAssociation;
    }
    ProgramWordWithoutAssociation otherTypeOfWord = parseProgramWordWithoutAssociation(word);
    return variant_cast(otherTypeOfWord);
}

ProgramWordWithoutAssociation parseProgramWordWithoutAssociation(const std::string& word) {
    if (auto potentialParenthesesGroup = tryParseParenthesesGroup(word)) {
        return *potentialParenthesesGroup;
    }
    if (auto potentialSquareBracketsGroup = tryParseSquareBracketsGroup(word)) {
        return *potentialSquareBracketsGroup;
    }
    if (auto potentialCurlyBracketsGroup = tryParseCurlyBracketsGroup(word)) {
        return *potentialCurlyBracketsGroup;
    }
    return parseAtom(word);
}

std::optional<Association*> tryParseAssociation(const std::string& word) {
    std::optional<Association*> res;
    auto i = word.find(':');
    if (i != std::string::npos) {
        std::string firstWordStr = word.substr(0, i - 1);
        std::string secondWordStr = word.substr(i + 1);
        ProgramWordWithoutAssociation firstWord = parseProgramWordWithoutAssociation(firstWordStr);
        ProgramWord secondWord = parseProgramWord(secondWordStr);
        res = new Association{firstWord, secondWord};
    }
    return res;
}

std::optional<ParenthesesGroup*> tryParseParenthesesGroup(const std::string& word) {
    std::optional<ParenthesesGroup*> res;
    if (word[0] == '(' && word[word.length() - 1] == ')') {
        std::string inner = word.substr(1, word.length() - 2);
        std::vector<Atom*> atoms;
        char* currentAtom;
        while ((currentAtom = strtok(inner.data(), ", ")) != nullptr) {
            atoms.push_back(parseAtom(std::string(currentAtom)));
        }
        res = new ParenthesesGroup{atoms};
    }
    return res;
}

std::optional<SquareBracketsGroup*> tryParseSquareBracketsGroup(const std::string& word) {
    std::optional<SquareBracketsGroup*> res;
    if (word[0] == '[' && word[word.length() - 1] == ']') {
        std::string inner = word.substr(1, word.length() - 2);
        std::vector<Atom*> atoms;
        char* currentAtom;
        while ((currentAtom = strtok(inner.data(), ", ")) != nullptr) {
            atoms.push_back(parseAtom(std::string(currentAtom)));
        }
        res = new SquareBracketsGroup{atoms};
    }
    return res;
}

std::optional<CurlyBracketsGroup*> tryParseCurlyBracketsGroup(const std::string& word) {
    std::optional<CurlyBracketsGroup*> res;
    if (word[0] == '{' && word[word.length() - 1] == '}') {
        std::string inner = word.substr(1, word.length() - 2);
        std::vector<ProgramSentence*> sentences;
        std::string currentSentence;
        while (std::getline(std::istringstream(inner), currentSentence)) {
            sentences.push_back(parseProgramSentence(currentSentence));
        }
        res = new CurlyBracketsGroup{sentences};
    }
    return res;
}

Atom* parseAtom(const std::string& atom) {
    return new Atom{atom};
}
