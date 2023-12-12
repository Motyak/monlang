#include <parse-functions.h>

#include <iostream>
#include <sstream>

std::istringstream slurpInputStream(std::istream& inputStream) {
    std::string res = "";
    std::string currentLine;
    while (std::getline(inputStream, currentLine)) {
        res += currentLine + '\n';
    }
    return std::istringstream(res);
}

int main()
{
    std::istringstream input = slurpInputStream(std::cin);
    
    // parseWord()

}
