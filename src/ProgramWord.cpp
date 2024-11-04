#include <monlang/ProgramWord.h>
#include <monlang/Word.h>
#include <monlang/SquareBracketsTerm.h>
#include <monlang/common.h>

MayFail<ProgramWord> consumeProgramWord(std::istringstream& input) {

    #ifndef DISABLE_SBT
    if (peekSequence(SquareBracketsTerm::INITIATOR_SEQUENCE, input)) {
        // we use _convert instead of _cast because SBT isn't a composable word
        return mayfail_convert<ProgramWord>(consumeSquareBracketsTerm(input));
    }
    #endif

    return mayfail_cast<ProgramWord>(consumeWord(input));
}
