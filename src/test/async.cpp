// THIS TEST FILE SHOULD NOT BE PART OF THE TEST SUITE (all.elf)

#include <monlang/CurlyBracketsGroup.h>
#include <monlang/common.h>

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

#include <future>
#include <chrono>
#include <thread>
#include <random>

using namespace std::chrono_literals;

TEST_CASE ("std::async tasks with 'async' policy", "[test-2000][async]") {
    auto input = tommy_str(R"EOF(
       |{
       |    {
       |        fds
       |    }
       |    sdf
       |}
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> CurlyBracketsGroup
       |  -> ProgramSentence #1
       |    -> ProgramWord: CurlyBracketsGroup
       |      -> ProgramSentence
       |        -> ProgramWord: Atom: `fds`
       |  -> ProgramSentence #2
       |    -> ProgramWord: Atom: `sdf`
    )EOF");

    constexpr int NB_OF_TASKS = 50;

    std::istringstream input_iss[NB_OF_TASKS];
    std::future<consumeCurlyBracketsGroup_RetType> future_output[NB_OF_TASKS];
    consumeCurlyBracketsGroup_RetType output[NB_OF_TASKS];

    // intialize all input_iss
    for (int i = 0; i < NB_OF_TASKS; ++i) {
        input_iss[i] = std::istringstream(input);
    }

    auto task = [&input_iss](int i){
        std::this_thread::sleep_for(500ms);
        std::cerr << ("start i=" + std::to_string(i) + "\n");
        auto res = consumeCurlyBracketsGroup(input_iss[i]);
        std::cerr << ("end i=" + std::to_string(i) + "\n");
        return res;
    };

    // launch async tasks
    for (int i = 0; i < NB_OF_TASKS; ++i) {
        future_output[i] = std::async(task, i);
    }

    // wait for async tasks to complete
    for (int i = 0; i < NB_OF_TASKS; ++i) {
        output[i] = future_output[i].get();
    }

    /* check each task result sequentially */
    for (int i = 0; i < NB_OF_TASKS; ++i) {
        auto output_word = mayfail_cast<ProgramWord_>(output[i]);
        auto output_str = montree::astToString(output_word);
        REQUIRE (output_str == expect);
    }
}
