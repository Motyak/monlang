
#include <grammar.h>
#include <variant-utils.h>

#include <variant>
#include <iostream>

int main()
{
    {
        std::istringstream iss("[123, [k1:v1, k2:v2]]");
        auto res = tryConsumeSquareBracketsGroup(iss);
        if (res) {
            std::visit(overload{
                [](SquareBracketsGroup* sbg) {
                    for (auto word: sbg->words) {
                        std::visit(overload{
                            [](Atom atom){std::cout << "Atom: ```" << atom.value << "```" << std::endl;},
                            [](SquareBracketsGroup* sbg) {
                                std::cout << "SquareBracketsGroup:" << std::endl;
                                for (auto word: sbg->words) {
                                    std::cout << "Association (left part): ```" << std::get<Atom>(std::get<Association*>(word)->leftPart).value << "```" << std::endl;
                                    std::cout << "Association (right part): ```" << std::get<Atom>(std::get<Association*>(word)->rightPart).value << "```" << std::endl;
                                }
                            },
                            [](auto other){std::cout << "other" << std::endl;}
                        }, word);
                    }
                },
                [](Association* assoc) { std::cout << "association" << std::endl; },
                [](auto other) { std::cerr << "wtf" << std::endl; }
            }, *res);
        } else {
            std::cout << "nothing" << std::endl;
        }
    }
}
