#ifndef TOMMYSTRING_H
#define TOMMYSTRING_H

#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>

#define tommy_str(raw_str) tommy_str(__FILE__, __LINE__, raw_str)
inline std::string (tommy_str)(std::string filename, int lineno, std::string raw_str) {
    /* /!\ only support indentation using tabs or 4 spaces times n */

    constexpr char TAB = 9;
    constexpr char SPACE = 32;

    unsigned indentation_level = 0;
    bool is_indented_with_tabs = false; // could have left it uninitialized, doesn't matter
    {
        auto ifs = std::ifstream(filename);
        for(int i = 1; i <= lineno - 1; ++i) {
            ifs.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
        char first_char = ifs.peek();
        if (first_char != TAB && first_char != SPACE) {
            goto END_OF_BLOCK;
        }
        is_indented_with_tabs = first_char == TAB;
        if (is_indented_with_tabs) {
            do {
                ifs.ignore();
                indentation_level++;
            } while (ifs.peek() == TAB);
        } else {
            int spaces = 0;
            do {
                ifs.ignore();
                if ((++spaces % 4) == 0) {
                    indentation_level++;
                }
            } while (ifs.peek() == SPACE);
        }

        END_OF_BLOCK:
        ;
    }

    std::string res("");
    std::istringstream iss(raw_str);
    iss.ignore(); // initial newline

    std::string cur_line;
    if (is_indented_with_tabs) {
        while (std::getline(iss, cur_line) && cur_line.size() > indentation_level * 1) {
            res += cur_line.substr((indentation_level + 1) * 1) + "\n";
        }
    } else {
        while (std::getline(iss, cur_line) && cur_line.size() > indentation_level * 4) {
            res += cur_line.substr((indentation_level + 1) * 4) + "\n";
        }
    }

    return res.substr(0, res.size() - 1);
}

#ifdef TOMMYSTRING_H_MAIN
int main()
{
    std::string abc = tommy_str(R"END(
        a
        b
        c
    )END");

    std::cout << "abc:\n`" << abc << "`\n"; /*
    abc:
    `a
    b
    c`
    */

    std::cout << std::endl;

        std::string fds = tommy_str(R"END(
            f
                d
                    s
        )END");

    std::cout << "fds:\n`" << fds << "`\n"; /*
    fds:
    `f
        d
            s`
    */

   std::cout << std::endl;

    std::string cpp = tommy_str(R"END(
        #define test
        #ifdef test
        #   error "test is defined"
        #endif
        
        int main()
        {
            // saying hello
            std::cout << "hello, world!\n";
        }
    )END");

    std::cout << "cpp:\n`" << cpp << "`\n"; /*
    cpp:
    `#define test
    #ifdef test
    #   error "test is defined"
    #endif

    int main()
    {
        // saying hello
        std::cout << "hello, world!\n";
    }`
    */
}
#endif // TOMMYSTRING_H_MAIN

#endif //TOMMYSTRING_H
