#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

#include "../src/ast/visitors/ConvenientVisitor.cpp"
#include "../src/ast/visitors/Unparse.cpp"

#include <random>

TEST_CASE ("random", "[test-5000][random]") {
    int seed = std::random_device{}();
    std::cerr << "[test-5000] seed is: `" << seed << "`" << std::endl;

    auto prog = montree::generateLV1Program(seed);
    std::ostringstream unparsed;
    visitAst(Unparse(unparsed), prog);
    auto output_str = montree::astToString(MayFail_<Program>(prog));
    REQUIRE (unparsed.str() == output_str);
}
