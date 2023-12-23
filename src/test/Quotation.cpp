#include <Quotation.h>

#include <catch2/catch_amalgamated.hpp>

#define DOUBLE_QUOTE (std::string(1, '"'))

/************************************************/
// ONELINE
/************************************************/

TEST_CASE("ERROR unexpected EOF while entering a quotation")
{
    std::istringstream iss(DOUBLE_QUOTE);
    REQUIRE_THROWS(tryConsumeQuotation(iss));
}

TEST_CASE("ERROR expected end of quotation but hit EOF")
{
    std::istringstream iss(DOUBLE_QUOTE + "fds");
    REQUIRE_THROWS(tryConsumeQuotation(iss));
}

TEST_CASE("ERROR expected end of quotation but hit EOF (backslashed double quote)")
{
    std::istringstream iss(DOUBLE_QUOTE + "fds" + BACKSLASH + DOUBLE_QUOTE);
    REQUIRE_THROWS(tryConsumeQuotation(iss));
}

TEST_CASE("empty quotation")
{
    std::istringstream iss(DOUBLE_QUOTE + DOUBLE_QUOTE);
    auto res = std::get<Quotation*>(*tryConsumeQuotation(iss));
    REQUIRE (res->quoted.value == "");
}

TEST_CASE("escaped double quote (containing backslashed double quote)")
{
    std::istringstream iss(DOUBLE_QUOTE + BACKSLASH + DOUBLE_QUOTE + DOUBLE_QUOTE);
    auto res = std::get<Quotation*>(*tryConsumeQuotation(iss));
    REQUIRE (res->quoted.value ==  DOUBLE_QUOTE);
}

TEST_CASE("double baskslash double quote")
{
    std::istringstream iss(DOUBLE_QUOTE 
            + BACKSLASH
            + BACKSLASH + DOUBLE_QUOTE
            + DOUBLE_QUOTE);
    auto res = std::get<Quotation*>(*tryConsumeQuotation(iss));
    REQUIRE (res->quoted.value ==  BACKSLASH + DOUBLE_QUOTE);
} 

/************************************************/
// MULTILINE
/************************************************/
