#include <catch2/catch_amalgamated.hpp>

#include <SquareBracketsGroup.h>

TEST_CASE("err-105 premature EOF after SquareBracketsGroup's initiator", "[test-1000][strictly]")
{
    std::istringstream iss(
        "["
    );

    auto res = tryConsumeSquareBracketsGroup(iss);

    REQUIRE (res);
    REQUIRE (!*res);
    REQUIRE (res->error().code == 105);
}



// TEST_CASE("no SquareBracketsGroup expression found", "[test-1050][strictly]")
// {
//     std::istringstream iss(
//         "fds"
//     );

//     auto res = tryConsumeSquareBracketsGroup(iss);

//     REQUIRE (!res);
// }
