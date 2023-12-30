#include <Quotation.h>

#include <catch2/catch_amalgamated.hpp>

#define TRIPLE_BACKTICK (std::string(3, '`'))

TEST_CASE("ERROR unexpected EOF while entering a quotation", "[multiline]")
{
    std::istringstream iss(TRIPLE_BACKTICK);
    REQUIRE_THROWS(tryConsumeQuotation(iss));
}

TEST_CASE("ERROR expected end of quotation but hit EOF", "[multiline]")
{
    std::istringstream iss(TRIPLE_BACKTICK + NEWLINE 
            + TRIPLE_BACKTICK);
    REQUIRE_THROWS(tryConsumeQuotation(iss));
}

TEST_CASE("ERROR was expecting newline after quoted format", "[multiline]")
{
    std::istringstream iss(TRIPLE_BACKTICK + "myquotedformat");
    REQUIRE_THROWS(tryConsumeQuotation(iss));
}

TEST_CASE("empty quotation", "[multiline]")
{
    std::istringstream iss(TRIPLE_BACKTICK + NEWLINE 
            + NEWLINE 
            + TRIPLE_BACKTICK);
    auto res = std::get<Quotation*>(*tryConsumeQuotation(iss));
    REQUIRE (res->quoted.value == "");
}

TEST_CASE("single line", "[multiline]")
{
    std::istringstream iss(TRIPLE_BACKTICK + NEWLINE 
            + "fds sdf" + NEWLINE 
            + TRIPLE_BACKTICK);
    auto res = std::get<Quotation*>(*tryConsumeQuotation(iss));
    REQUIRE (res->quoted.value == "fds sdf");
}

TEST_CASE("multiple lines", "[multiline]")
{
    std::istringstream iss(TRIPLE_BACKTICK + NEWLINE 
            + "fds" + NEWLINE 
            + "sdf" + NEWLINE 
            + TRIPLE_BACKTICK);
    auto res = std::get<Quotation*>(*tryConsumeQuotation(iss));
    REQUIRE (res->quoted.value == "fds\nsdf");
}

TEST_CASE("quoted format", "[multiline]")
{
    std::istringstream iss(TRIPLE_BACKTICK + "myquotedformat" + NEWLINE 
            + "fds" + NEWLINE 
            + "sdf" + NEWLINE 
            + TRIPLE_BACKTICK);
    auto res = std::get<Quotation*>(*tryConsumeQuotation(iss));
    REQUIRE (res->quotedFormat == "myquotedformat");
    REQUIRE (res->quoted.value == "fds\nsdf");
}
