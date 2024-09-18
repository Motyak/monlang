#include <monlang/CurlyBracketsGroup.h>
#include <monlang/common.h>
#include <monlang/ProgramSentence.h>

const Sequence CurlyBracketsGroup::INITIATOR_SEQUENCE = {'{'};
const Sequence CurlyBracketsGroup::TERMINATOR_SEQUENCE = {'}'};
const std::vector<char> CurlyBracketsGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<CurlyBracketsGroup> consumeCurlyBracketsGroup(int indentLevel, std::istringstream& input) {
    TRACE_CUR_FUN();
    std::vector<MayFail<ProgramSentence>> sentences;

    // TODO

    return CurlyBracketsGroup{sentences};







    // TRACE_CUR_FUN();
    // std::vector<MayFail<ProgramSentence>> sentences;
    // MayFail<ProgramSentence> currentSentence;
    // while (input.peek() != EOF) {
    //     currentSentence = consumeProgramSentence(input);
    //     sentences.push_back(currentSentence);
    //     if (!currentSentence.has_value()) {
    //         return std::unexpected(Malformed(Program{sentences}, Error{119}));
    //     }
    // }
    // return Program{sentences};
}
