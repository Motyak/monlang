#include <common.h>

#include <iostream>

void consumeSequence(std::vector<char> sequence, std::istringstream& input) {
    for (auto c: sequence) {
        if (input.peek() != c) {
            if (input.peek() == -1) {
                std::cerr << "was expecting `" << c << "` but hit EOF" << std::endl;
            } else {
                std::cerr << "was expecting `" << c 
                        << "` but found `" << char(input.peek()) << "`" << std::endl;
            }
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
