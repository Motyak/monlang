#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

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

MayFail_<Program> Program::wrap() const {
    return MayFail_<Program>{vec_cast<MayFail<MayFail_<ProgramSentence>>>(this->sentences)};
}

MayFail_<Program>::MayFail_(std::vector<MayFail<MayFail_<ProgramSentence>>> sentences) : sentences(sentences){}

MayFail_<Program>::MayFail_(Program prog) : MayFail_(prog.wrap()){}

MayFail_<Program>::operator Program() const {
    return Program{vec_cast<ProgramSentence>(sentences)};
}

Program MayFail_<Program>::unwrap() const {
    return (Program)*this;
}
