// #define DEBUG //toggle

#ifdef PROGRAM_H
#ifdef DEBUG
#warning PROGRAM_H
#endif
extern template class Malformed<Program>;
extern template class std::expected<Program, Malformed<Program>>;
extern template const Program& mayfail_unwrap(const MayFail<Program>&);
extern template Program& mayfail_unwrap(MayFail<Program>&);
extern template std::string serializeErr(MayFail<Program>);
#endif

#ifdef PROGRAM_SENTENCE_H
#ifdef DEBUG
#warning PROGRAM_SENTENCE_H
#endif
extern template class Malformed<ProgramSentence>;
extern template class std::expected<ProgramSentence, Malformed<ProgramSentence>>;
extern template const ProgramSentence& mayfail_unwrap(const MayFail<ProgramSentence>&);
extern template ProgramSentence& mayfail_unwrap(MayFail<ProgramSentence>&);
extern template std::string serializeErr(MayFail<ProgramSentence>);
#endif

#ifdef PROGRAM_WORD_H
#ifdef DEBUG
#warning PROGRAM_WORD_H
#endif
extern template class Malformed<ProgramWord>;
extern template class std::expected<ProgramWord, Malformed<ProgramWord>>;
extern template MayFail<ProgramWord> mayfail_cast(MayFail<Word>);
extern template MayFail<Word> mayfail_cast(MayFail<ProgramWord>);
extern template const ProgramWord& mayfail_unwrap(const MayFail<ProgramWord>&);
extern template ProgramWord& mayfail_unwrap(MayFail<ProgramWord>&);
extern template std::string serializeErr(MayFail<ProgramWord>);
#endif

#ifdef TERM_H
#ifdef DEBUG
#warning TERM_H
#endif
extern template class Malformed<Term>;
extern template class std::expected<Term, Malformed<Term>>;
extern template const Term& mayfail_unwrap(const MayFail<Term>&);
extern template Term& mayfail_unwrap(MayFail<Term>&);
extern template std::string serializeErr(MayFail<Term>);
#endif

#ifdef WORD_H
#ifdef DEBUG
#warning WORD_H
#endif
extern template class Malformed<Word>;
extern template class std::expected<Word, Malformed<Word>>;
extern template MayFail<Word> mayfail_cast(MayFail<Atom*>);
extern template const Word& mayfail_unwrap(const MayFail<Word>&);
extern template Word& mayfail_unwrap(MayFail<Word>&);
extern template std::string serializeErr(MayFail<Word>);
#endif

////////////////////////////////////////////////////////////////

#ifdef ATOM_H
#ifdef DEBUG
#warning ATOM_H
#endif
extern template class Malformed<Atom>;
extern template class std::expected<Atom, Malformed<Atom>>;
extern template const Atom& mayfail_unwrap(const MayFail<Atom>&);
extern template Atom& mayfail_unwrap(MayFail<Atom>&);
extern template std::string serializeErr(MayFail<Atom>);
#endif

#ifdef PARENTHESES_GROUP_H
#ifdef DEBUG
#warning PARENTHESES_GROUP_H
#endif
extern template class Malformed<ParenthesesGroup>;
extern template class std::expected<ParenthesesGroup, Malformed<ParenthesesGroup>>;
extern template MayFail<Word> mayfail_cast(MayFail<ParenthesesGroup*>);
extern template MayFail<Word> mayfail_convert(MayFail<ParenthesesGroup>);
extern template const ParenthesesGroup& mayfail_unwrap(const MayFail<ParenthesesGroup>&);
extern template ParenthesesGroup& mayfail_unwrap(MayFail<ParenthesesGroup>&);
extern template std::string serializeErr(MayFail<ParenthesesGroup>);
#endif

#ifdef SQUARE_BRACKETS_GROUP_H
#ifdef DEBUG
#warning SQUARE_BRACKETS_GROUP_H
#endif
extern template class Malformed<SquareBracketsGroup>;
extern template class std::expected<SquareBracketsGroup, Malformed<SquareBracketsGroup>>;
extern template MayFail<Word> mayfail_cast(MayFail<SquareBracketsGroup*>);
extern template MayFail<Word> mayfail_convert(MayFail<SquareBracketsGroup>);
extern template const SquareBracketsGroup& mayfail_unwrap(const MayFail<SquareBracketsGroup>&);
extern template SquareBracketsGroup& mayfail_unwrap(MayFail<SquareBracketsGroup>&);
extern template std::string serializeErr(MayFail<SquareBracketsGroup>);
#endif

#ifdef CURLY_BRACKETS_GROUP_H
#ifdef DEBUG
#warning CURLY_BRACKETS_GROUP_H
#endif
extern template class Malformed<CurlyBracketsGroup>;
extern template class std::expected<CurlyBracketsGroup, Malformed<CurlyBracketsGroup>>;
extern template MayFail<Word> mayfail_cast(MayFail<CurlyBracketsGroup*>);
extern template MayFail<Word> mayfail_convert(MayFail<CurlyBracketsGroup>);
extern template const CurlyBracketsGroup& mayfail_unwrap(const MayFail<CurlyBracketsGroup>&);
extern template CurlyBracketsGroup& mayfail_unwrap(MayFail<CurlyBracketsGroup>&);
extern template std::string serializeErr(MayFail<CurlyBracketsGroup>);
#endif

#ifdef POSTFIX_PARENTHESES_GROUP_H
#ifdef DEBUG
#warning POSTFIX_PARENTHESES_GROUP_H
#endif
extern template class Malformed<PostfixParenthesesGroup>;
extern template class std::expected<PostfixParenthesesGroup, Malformed<PostfixParenthesesGroup>>;
extern template MayFail<Word> mayfail_cast(MayFail<PostfixParenthesesGroup*>);
extern template MayFail<Word> mayfail_convert(MayFail<PostfixParenthesesGroup>);
extern template const PostfixParenthesesGroup& mayfail_unwrap(const MayFail<PostfixParenthesesGroup>&);
extern template PostfixParenthesesGroup& mayfail_unwrap(MayFail<PostfixParenthesesGroup>&);
extern template std::string serializeErr(MayFail<PostfixParenthesesGroup>);
#endif

#ifdef POSTFIX_SQUARE_BRACKETS_GROUP_H
#ifdef DEBUG
#warning POSTFIX_SQUARE_BRACKETS_GROUP_H
#endif
extern template class Malformed<PostfixSquareBracketsGroup>;
extern template class std::expected<PostfixSquareBracketsGroup, Malformed<PostfixSquareBracketsGroup>>;
extern template MayFail<Word> mayfail_cast(MayFail<PostfixSquareBracketsGroup*>);
extern template MayFail<Word> mayfail_convert(MayFail<PostfixSquareBracketsGroup>);
extern template const PostfixSquareBracketsGroup& mayfail_unwrap(const MayFail<PostfixSquareBracketsGroup>&);
extern template PostfixSquareBracketsGroup& mayfail_unwrap(MayFail<PostfixSquareBracketsGroup>&);
extern template std::string serializeErr(MayFail<PostfixSquareBracketsGroup>);
#endif
