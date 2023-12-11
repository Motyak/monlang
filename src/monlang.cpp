#include <grammar-structs.h>

#include <sstream>
#include <iostream>
#include <optional>

ProgramWord parseProgramWord(const std::string& word);

ProgramWordWithoutAssociation parseProgramWordWithoutAssociation(const std::string& word) {

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

ProgramWord parseProgramWord(const std::string& word) {
    


    return new Atom{word};
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

        res.push_back(parseProgramWord(word));
    }

    return new ProgramSentence{res};
}

Program parseProgram(std::istream& inputStream) {
    std::vector<ProgramSentence*> res;

    std::vector<std::string> sentences;
    std::string currentSentence;

    while (std::getline(inputStream, currentSentence)) {
        sentences.push_back(currentSentence);
    }

    for (auto& sentence: sentences) {
        /* remove comments and extra whitespaces */
        // TODO: 

        res.push_back(parseProgramSentence(sentence));
    }

    return Program{res};
}

int main()
{
    Program program = parseProgram(std::cin);

    // std::vector<std::string> lines = readStdinLines();

    // ::Program program = lines.parse();
}
