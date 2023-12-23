#include <common.h>
#include <utils/str-utils.h>

#include <iostream>

void consumeSequence(std::vector<char> sequence, std::istringstream& input) {
    for (auto c: sequence) {
        if (input.peek() != c) {
                std::cerr << "was expecting " << str(c) 
                        << " but found " << str(input.peek()) << std::endl;
            throw std::runtime_error("user exception");
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
