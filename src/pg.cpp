
#include <grammar.h>
#include <variant-utils.h>

#include <variant>
#include <iostream>

int main()
{
    {
        std::istringstream iss("[123]");
        auto res = tryConsumeSquareBracketsGroup(iss);
        if (res) {
            std::visit(overload{
                [](SquareBracketsGroup* sbg){ std::cout << "square brackets group" << std::endl; },
                [](Association* assoc) { std::cout << "association" << std::endl; },
                [](auto other) { std::cerr << "wtf" << std::endl; }
            }, *res);
        } else {
            std::cout << "nothing" << std::endl;
        }
    }
}
