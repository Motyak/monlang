
#include <grammar.h>
#include <variant-utils.h>

#include <sstream>
#include <vector>
#include <iostream>

#define SPACE (char(32))
#define NEWLINE (char(10))

const std::vector<char> ProgramSentence::CONTINUATOR_SEQUENCE = { SPACE };
const std::vector<char> ProgramSentence::TERMINATOR_SEQUENCE = { NEWLINE };

const std::vector<char> ParenthesesGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const std::vector<char> ParenthesesGroup::ALT_CONTINUATOR_SEQUENCE = { SPACE };
const std::vector<char> ParenthesesGroup::TERMINATOR_SEQUENCE = { ')' };

const std::vector<char> SquareBracketsGroup::INITIATOR_SEQUENCE = { '[' };
const std::vector<char> SquareBracketsGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const std::vector<char> SquareBracketsGroup::TERMINATOR_SEQUENCE = { ']' };

const std::vector<char> Association::SEPARATOR_SEQUENCE = { ':' };

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
    if (!input) {
        std::cerr << "was expecting at least one word in ProgramSentence, found none" << std::endl;
        exit(1);
    }
    auto currentWord = consumeProgramWord(input);
    words.push_back(currentWord);
    while (input && !peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        consumeSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input);
        currentWord = consumeProgramWord(input);
        words.push_back(currentWord);
    }
    return ProgramSentence{words};
}

ProgramWord consumeProgramWord(std::istringstream& input) {
    // if (auto potentialParenthesesGroup = tryConsumeParenthesesGroup(input)) {
    //     return *potentialParenthesesGroup;
    // }
    // if (auto potentialSquareBracketsGroup = tryConsumeSquareBracketsGroup(input)) {
    //     return variant_cast(*potentialSquareBracketsGroup);
    // }
    // if (auto potentialQuotation = tryConsumeQuotation(input)) {
    //     return *potentialQuotation;
    // }
    // if (auto potentialCurlyBracketsGroup = tryConsumeCurlyBracketsGroup(input)) {
    //     return *potentialCurlyBracketsGroup;
    // }
    return consumeAtom(input);
}

ProgramWordWithoutAssociation consumeProgramWordWithoutAssociation(std::istringstream& input) {
    // if (auto potentialParenthesesGroup = tryConsumeParenthesesGroup(input, AllowAssociation::FALSE)) {
    //     return *potentialParenthesesGroup;
    // }
    // if (auto potentialSquareBracketsGroup = tryConsumeSquareBracketsGroup(input, AllowAssociation::FALSE)) {
    //     // since we passed the AllowAssociation::FALSE flag, we know we got a SquareBracketsGroup
    //     return ProgramWordWithoutAssociation{std::get<SquareBracketsGroup*>(*potentialSquareBracketsGroup)};
    // }
    // if (auto potentialQuotation = tryConsumeQuotation(input, AllowAssociation::FALSE)) {
    //     return *potentialQuotation;
    // }
    // if (auto potentialCurlyBracketsGroup = tryConsumeCurlyBracketsGroup(input, AllowAssociation::FALSE)) {
    //     return *potentialCurlyBracketsGroup;
    // }
    return consumeAtom(input, AllowAssociation::FALSE); // we should hit a space or newline here ? do we check inside and consume after fcall ?
}

std::optional<ParenthesesGroup*> tryConsumeParenthesesGroup(std::istringstream&, AllowAssociation flag) {

}

std::optional<std::variant<SquareBracketsGroup*, Association*>> tryConsumeSquareBracketsGroup(std::istringstream& input, AllowAssociation flag) {
    if (!peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return {};
    }
    input.ignore(SquareBracketsGroup::INITIATOR_SEQUENCE.size()); // consume initiator characters

    std::vector<ProgramWord> words;

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while about to parse a program word in square brackets group" << std::endl;
        exit(1);
    }

    if (peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(SquareBracketsGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters
        return new SquareBracketsGroup{words}; // empty
    }

    auto currentWord = consumeProgramWord(input);
    words.push_back(currentWord);
    while (input
            && (!peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input))
            && (!peekSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input))
            && (!peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input))) {
        consumeSequence(SquareBracketsGroup::CONTINUATOR_SEQUENCE, input);
        currentWord = consumeProgramWord(input);
        words.push_back(currentWord);
    }

    if (!peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        auto& ts = SquareBracketsGroup::TERMINATOR_SEQUENCE;
        std::cerr << "was expecting `" 
                << std::string(ts.begin(), ts.end()) 
                << "` but found `" << input.peek() << "`" << std::endl;
        exit(1);
    }
    input.ignore(SquareBracketsGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters

    if (flag == AllowAssociation::TRUE && peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
        ProgramWordWithoutAssociation leftPart = new SquareBracketsGroup{words};
        input.ignore(Association::SEPARATOR_SEQUENCE.size()); // consume association separator characters
        ProgramWord rightPart = consumeProgramWord(input);
        return new Association{leftPart, rightPart};
    }

    return new SquareBracketsGroup{words};
}

std::optional<Quotation> tryConsumeQuotation(std::istringstream& input, AllowAssociation flag) {
    
}

std::optional<CurlyBracketsGroup*> tryConsumeCurlyBracketsGroup(std::istringstream& input, AllowAssociation flag) {
    
}

// std::optional<Association*> tryConsumeAssociation(std::istringstream& input) {
    
// }

Quoted consumeQuoted(std::istringstream& input, AllowAssociation flag) {
    
}

Atom consumeAtom(std::istringstream& input, AllowAssociation flag) {
    
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

bool peekSequence(std::vector<char> sequence, std::istringstream& input) {
    // save stream position
    std::streampos initialPosition = input.tellg();

    // peek, check, consume      in a loop
    for (auto c: sequence) {
        if (input.peek() != c) {
            // restore stream position
            input.seekg(initialPosition);
            return false;
        }
        input.ignore(1);
    }

    // restore stream position
    input.seekg(initialPosition);
    return true;
}
