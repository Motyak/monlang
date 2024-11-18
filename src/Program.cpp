#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

#include <utils/vec-utils.h>

MayFail<MayFail_<Program>> consumeProgram(std::istringstream& input) {
    TRACE_CUR_FUN();

    std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;
    MayFail<MayFail_<ProgramSentence>> currentSentence;

    while (input.peek() != EOF) {
        currentSentence = consumeProgramSentence(input);

        if (!currentSentence.has_error() && currentSentence.value().programWords.size() == 0) {
            continue; // ignore empty sentences
        }

        sentences.push_back(currentSentence);

        if (currentSentence.has_error()) {
            return Malformed(MayFail_<Program>{sentences}, ERR(119));
        }
    }

    return MayFail_<Program>{sentences};
}

///////////////////////////////////////////////////////////

template <>
Program unwrap(const MayFail_<Program>& prog) {
    return (Program)prog;
}

template <>
MayFail_<Program> wrap(const Program& prog) {
    return MayFail_<Program>{vec_cast<MayFail<MayFail_<ProgramSentence>>>(prog.sentences)};
}

MayFail_<Program>::MayFail_(std::vector<MayFail<MayFail_<ProgramSentence>>> sentences) : sentences(sentences){}

MayFail_<Program>::MayFail_(Program prog) : MayFail_(wrap(prog)){}

MayFail_<Program>::operator Program() const {
    return Program{vec_cast<ProgramSentence>(sentences)};
}
