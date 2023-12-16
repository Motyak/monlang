
#include <grammar.h>
#include <vec-utils.h>
#include <variant-utils.h>

#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

#define SPACE (char(32))
#define NEWLINE (char(10))

const std::vector<char> ProgramSentence::CONTINUATOR_SEQUENCE = { SPACE };
const std::vector<char> ProgramSentence::TERMINATOR_SEQUENCE = { NEWLINE };
const std::vector<std::vector<char>> ProgramSentence::RESERVED_SEQUENCES = {
    { SPACE },
    { NEWLINE },
};

const std::vector<char> ParenthesesGroup::INITIATOR_SEQUENCE = { '(' };
const std::vector<char> ParenthesesGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const std::vector<char> ParenthesesGroup::ALT_CONTINUATOR_SEQUENCE = { SPACE };
const std::vector<char> ParenthesesGroup::TERMINATOR_SEQUENCE = { ')' };
const std::vector<std::vector<char>> ParenthesesGroup::RESERVED_SEQUENCES = {
    { '(' },
    { ',' },
    { SPACE },
    { ')' },
};

const std::vector<char> SquareBracketsGroup::INITIATOR_SEQUENCE = { '[' };
const std::vector<char> SquareBracketsGroup::CONTINUATOR_SEQUENCE = { ',', SPACE };
const std::vector<char> SquareBracketsGroup::TERMINATOR_SEQUENCE = { ']' };
const std::vector<std::vector<char>> SquareBracketsGroup::RESERVED_SEQUENCES = {
    { '[' },
    { ',' },
    { SPACE },
    { ']' },
};

const std::vector<char> Association::SEPARATOR_SEQUENCE = { ':' };
const std::vector<std::vector<char>> Association::RESERVED_SEQUENCES = {
    { ':' },
};

const std::vector<char> Quotation::INITIATOR_SEQUENCE = { '"' };
const std::vector<char> Quotation::TERMINATOR_SEQUENCE = { '"' };
const std::vector<std::vector<char>> Quotation::RESERVED_SEQUENCES = {
    { '"' }
};

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
    if (auto potentialSquareBracketsGroup = tryConsumeSquareBracketsGroup(input)) {
        return variant_cast(*potentialSquareBracketsGroup);
    }
    // if (auto potentialQuotation = tryConsumeQuotation(input)) {
    //     return *potentialQuotation;
    // }
    // if (auto potentialCurlyBracketsGroup = tryConsumeCurlyBracketsGroup(input)) {
    //     return *potentialCurlyBracketsGroup;
    // }
    return variant_cast(consumeAtom(input));
}

std::optional<ParenthesesGroup*> tryConsumeParenthesesGroup(std::istringstream&) {

}

std::optional<std::variant<SquareBracketsGroup*, Association*>> tryConsumeSquareBracketsGroup(std::istringstream& input) {
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
        if (input.peek() == -1) {
            std::cerr << "was expecting `" 
                << std::string(ts.begin(), ts.end()) 
                << "` but hit EOF" << std::endl;
        } else {
            std::cerr << "was expecting `" 
                << std::string(ts.begin(), ts.end()) 
                << "` but found `" << char(input.peek()) << "`" << std::endl;
        }
        exit(1);
    }
    input.ignore(SquareBracketsGroup::TERMINATOR_SEQUENCE.size()); // consume terminator characters

    if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
        ProgramWordWithoutAssociation leftPart = new SquareBracketsGroup{words};
        input.ignore(Association::SEPARATOR_SEQUENCE.size()); // consume association separator characters
        ProgramWord rightPart = consumeProgramWord(input);
        return new Association{leftPart, rightPart};
    }

    return new SquareBracketsGroup{words};
}

std::optional<Quotation> tryConsumeQuotation(std::istringstream& input) {
    
}

std::optional<CurlyBracketsGroup*> tryConsumeCurlyBracketsGroup(std::istringstream& input) {
    
}

// std::optional<Association*> tryConsumeAssociation(std::istringstream& input) {
    
// }

Quoted consumeQuoted(std::istringstream& input) {
    
}

std::variant<Atom, Association*> consumeAtom(std::istringstream& input) {
    static const auto FORBIDDEN_SEQUENCES = vec_union({
        ProgramSentence::RESERVED_SEQUENCES,
        ParenthesesGroup::RESERVED_SEQUENCES,
        SquareBracketsGroup::RESERVED_SEQUENCES,
        Association::RESERVED_SEQUENCES,
        Quotation::RESERVED_SEQUENCES,
    });

    std::string value;

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while about to parse an atom value" << std::endl;
        exit(1);
    }

    char currentChar;
    while (input && std::all_of(
                FORBIDDEN_SEQUENCES.begin(),
                FORBIDDEN_SEQUENCES.end(),
                [&input](auto seq){return !peekSequence(seq, input);})) {
        input.get(currentChar);
        value += currentChar;
    }

    if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
        ProgramWordWithoutAssociation leftPart = Atom{value};
        input.ignore(Association::SEPARATOR_SEQUENCE.size()); // consume association separator characters
        ProgramWord rightPart = consumeProgramWord(input);
        return new Association{leftPart, rightPart};
    }

    if (value.size() == 0) {
        std::cerr << "expected an atom value but found nothing" << std::endl;
        exit(1);
    }

    return Atom{value};
}

void consumeSequence(std::vector<char> sequence, std::istringstream& input) {
    for (auto c: sequence) {
        if (input.peek() != c) {
            if (input.peek() == -1) {
                std::cerr << "was expecting `" << c << "` but hit EOF" << std::endl;
            } else {
                std::cerr << "was expecting `" << c 
                        << "` but found `" << char(input.peek()) << "`" << std::endl;
            }
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
