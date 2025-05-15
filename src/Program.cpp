#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

#include <utils/vec-utils.h>

MayFail<MayFail_<Program>> consumeProgram(std::istringstream& input) {
    TRACE_CUR_FUN();

    std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;
    MayFail<MayFail_<ProgramSentence>> currentSentence;

    auto newlines = size_t(0);
    while (input.peek() != EOF) {
        currentSentence = consumeProgramSentence(input);

        if (!currentSentence.has_error() && currentSentence.value().programWords.size() == 0) {
            newlines += currentSentence.value()._tokenLen;
            continue; // ignore empty sentences
        }

        currentSentence.val._tokenLeadingNewlines = newlines;
        newlines = 0;
        sentences.push_back(currentSentence);

        if (currentSentence.has_error()) {
            return Malformed(MayFail_<Program>{sentences}, ERR(119));
        }
    }

    // we don't care about trailing newlines in Program, only in CurlyBracketsGroup

    return MayFail_<Program>{sentences};
}

///////////////////////////////////////////////////////////

Program::Program(const std::vector<ProgramSentence>& sentences) : sentences(sentences){}

MayFail_<Program>::MayFail_(const std::vector<MayFail<MayFail_<ProgramSentence>>>& sentences) : sentences(sentences){}

MayFail_<Program>::MayFail_(const Program& prog) {
    this->sentences = vec_cast<MayFail<MayFail_<ProgramSentence>>>(prog.sentences);
    this->_tokenId = prog._tokenId;
}

MayFail_<Program>::operator Program() const {
    auto sentences = vec_cast<ProgramSentence>(this->sentences);
    auto prog = Program{sentences};
    prog._tokenId = this->_tokenId;
    return prog;
}
