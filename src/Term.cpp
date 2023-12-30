#include <Term.h>
#include <common.h>
#include <utils/str-utils.h>
#include <utils/assert-utils.h>

#include <iostream>
#include <algorithm>

const Sequence Term::CONTINUATOR_SEQUENCE = { SPACE };

const std::vector<char> Term::RESERVED_CHARACTERS = {
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value()
};

MayFail<Term> consumeTerm(std::istringstream& input, const std::vector<char>& terminatorCharacters) {
    if (input.peek() == EOF) {
        return std::unexpected(Error{114});
    }

    std::vector<ProgramWord> words;

    if (auto word = consumeProgramWord(input); !word) {
        return std::unexpected(Error{115, new Error{word.error()}});
    } else {

        words.push_back(*word);
    }

    while (input.peek() != EOF && std::all_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(), 
            [&input](auto terminatorChar){return input.peek() != terminatorChar;})) {
        if (auto res = consumeSequence(Term::CONTINUATOR_SEQUENCE, input); !res) {
            return std::unexpected(Error{116, new Error{res.error()}});
        }
        if (auto word = consumeProgramWord(input); !word) {
            return std::unexpected(Error{115, new Error{word.error()}});
        } else {
            words.push_back(*word);
        }
    }

    return Term{words};
}
