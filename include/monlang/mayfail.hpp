// SHOULD ONLY BE INCLUDED BY COMMON.H

#if 0 \
// +1 //toggle for IDE
    #include <monlang/Program.h>
    #include <monlang/ProgramSentence.h>
    #include <monlang/ProgramWord.h>
    #include <monlang/Term.h>
    #include <monlang/Word.h>

    #include <monlang/Atom.h>
    #include <monlang/ParenthesesGroup.h>
    #include <monlang/SquareBracketsGroup.h>
    #include <monlang/CurlyBracketsGroup.h>

    #include <monlang/PostfixParenthesesGroup.h>
    #include <monlang/PostfixSquareBracketsGroup.h>

    #include <string>
    #include <variant>
    #include <expected>
#endif

/* Malformed<T> */

extern template class Malformed<Program>;
extern template class Malformed<ProgramSentence>;
extern template class Malformed<ProgramWord>;
extern template class Malformed<Term>;
extern template class Malformed<Word>;

extern template class Malformed<Atom>;
extern template class Malformed<ParenthesesGroup>;
extern template class Malformed<SquareBracketsGroup>;
extern template class Malformed<CurlyBracketsGroup>;

extern template class Malformed<PostfixParenthesesGroup>;
extern template class Malformed<PostfixSquareBracketsGroup>;

/* using MayFail = std::expected<T, Malformed<T>> */

extern template class std::expected<Program, Malformed<Program>>;
extern template class std::expected<ProgramSentence, Malformed<ProgramSentence>>;
extern template class std::expected<ProgramWord, Malformed<ProgramWord>>;
extern template class std::expected<Term, Malformed<Term>>;
extern template class std::expected<Word, Malformed<Word>>;

extern template class std::expected<Atom, Malformed<Atom>>;
extern template class std::expected<ParenthesesGroup, Malformed<ParenthesesGroup>>;
extern template class std::expected<SquareBracketsGroup, Malformed<SquareBracketsGroup>>;
extern template class std::expected<CurlyBracketsGroup, Malformed<CurlyBracketsGroup>>;

extern template class std::expected<PostfixParenthesesGroup, Malformed<PostfixParenthesesGroup>>;
extern template class std::expected<PostfixSquareBracketsGroup, Malformed<PostfixSquareBracketsGroup>>;

/* MayFail<Word> mayfail_cast(MayFail<T> inputMayfail) */

extern template MayFail<Word> mayfail_cast(MayFail<ProgramWord>);

extern template MayFail<Word> mayfail_cast(MayFail<Atom>);
extern template MayFail<Word> mayfail_cast(MayFail<ParenthesesGroup>);
extern template MayFail<Word> mayfail_cast(MayFail<SquareBracketsGroup>);
extern template MayFail<Word> mayfail_cast(MayFail<CurlyBracketsGroup>);

extern template MayFail<Word> mayfail_cast(MayFail<PostfixParenthesesGroup>);
extern template MayFail<Word> mayfail_cast(MayFail<PostfixSquareBracketsGroup>);

// MayFail<ProgramWord> mayfail_cast(MayFail<Word> inputMayfail)
extern template MayFail<ProgramWord> mayfail_cast(MayFail<Word>);

/* MayFail<Word> mayfail_convert(MayFail<T> inputMayfail) */

extern template MayFail<Word> mayfail_convert(MayFail<Atom>);
extern template MayFail<Word> mayfail_convert(MayFail<ParenthesesGroup>);
extern template MayFail<Word> mayfail_convert(MayFail<SquareBracketsGroup>);
extern template MayFail<Word> mayfail_convert(MayFail<CurlyBracketsGroup>);

extern template MayFail<Word> mayfail_convert(MayFail<PostfixParenthesesGroup>);
extern template MayFail<Word> mayfail_convert(MayFail<PostfixSquareBracketsGroup>);

/* const T& mayfail_unwrap(const MayFail<T>& inputMayfail) */

extern template const Program& mayfail_unwrap(const MayFail<Program>&);
extern template const ProgramSentence& mayfail_unwrap(const MayFail<ProgramSentence>&);
extern template const ProgramWord& mayfail_unwrap(const MayFail<ProgramWord>&);
extern template const Term& mayfail_unwrap(const MayFail<Term>&);
extern template const Word& mayfail_unwrap(const MayFail<Word>&);

extern template const Atom& mayfail_unwrap(const MayFail<Atom>&);
extern template const ParenthesesGroup& mayfail_unwrap(const MayFail<ParenthesesGroup>&);
extern template const SquareBracketsGroup& mayfail_unwrap(const MayFail<SquareBracketsGroup>&);
extern template const CurlyBracketsGroup& mayfail_unwrap(const MayFail<CurlyBracketsGroup>&);

extern template const PostfixParenthesesGroup& mayfail_unwrap(const MayFail<PostfixParenthesesGroup>&);
extern template const PostfixSquareBracketsGroup& mayfail_unwrap(const MayFail<PostfixSquareBracketsGroup>&);

/* T& mayfail_unwrap(MayFail<T>& inputMayfail) */

extern template Program& mayfail_unwrap(MayFail<Program>&);
extern template ProgramSentence& mayfail_unwrap(MayFail<ProgramSentence>&);
extern template ProgramWord& mayfail_unwrap(MayFail<ProgramWord>&);
extern template Term& mayfail_unwrap(MayFail<Term>&);
extern template Word& mayfail_unwrap(MayFail<Word>&);

extern template Atom& mayfail_unwrap(MayFail<Atom>&);
extern template ParenthesesGroup& mayfail_unwrap(MayFail<ParenthesesGroup>&);
extern template SquareBracketsGroup& mayfail_unwrap(MayFail<SquareBracketsGroup>&);
extern template CurlyBracketsGroup& mayfail_unwrap(MayFail<CurlyBracketsGroup>&);

extern template PostfixParenthesesGroup& mayfail_unwrap(MayFail<PostfixParenthesesGroup>&);
extern template PostfixSquareBracketsGroup& mayfail_unwrap(MayFail<PostfixSquareBracketsGroup>&);

/* std::string serializeErr(MayFail<T> malformed) */

extern template std::string serializeErr(MayFail<Program>);
extern template std::string serializeErr(MayFail<ProgramSentence>);
extern template std::string serializeErr(MayFail<ProgramWord>);
extern template std::string serializeErr(MayFail<Term>);
extern template std::string serializeErr(MayFail<Word>);

extern template std::string serializeErr(MayFail<Atom>);
extern template std::string serializeErr(MayFail<ParenthesesGroup>);
extern template std::string serializeErr(MayFail<SquareBracketsGroup>);
extern template std::string serializeErr(MayFail<CurlyBracketsGroup>);

extern template std::string serializeErr(MayFail<PostfixParenthesesGroup>);
extern template std::string serializeErr(MayFail<PostfixSquareBracketsGroup>);
