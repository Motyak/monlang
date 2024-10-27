// THIS .TPP SHOULD ONLY BE INCLUDED BY ENTITIES .CPP
// THIS .TPP SHOULD NOT BE COMPILED DIRECTLY

#ifdef PROGRAM_CPP
template class Malformed<Program>;
template class std::expected<Program, Malformed<Program>>;
template const Program& mayfail_unwrap(const MayFail<Program>&);
template Program& mayfail_unwrap(MayFail<Program>&);
template std::string serializeErr(MayFail<Program>);
#endif

#ifdef PROGRAM_SENTENCE_CPP
template class Malformed<ProgramSentence>;
template class std::expected<ProgramSentence, Malformed<ProgramSentence>>;
template const ProgramSentence& mayfail_unwrap(const MayFail<ProgramSentence>&);
template ProgramSentence& mayfail_unwrap(MayFail<ProgramSentence>&);
template std::string serializeErr(MayFail<ProgramSentence>);
#endif

#ifdef PROGRAM_WORD_CPP
template class Malformed<ProgramWord>;
template class std::expected<ProgramWord, Malformed<ProgramWord>>;
template MayFail<Word> mayfail_cast(MayFail<ProgramWord>);
template const ProgramWord& mayfail_unwrap(const MayFail<ProgramWord>&);
template ProgramWord& mayfail_unwrap(MayFail<ProgramWord>&);
template std::string serializeErr(MayFail<ProgramWord>);
#endif

#ifdef TERM_CPP
template class Malformed<Term>;
template class std::expected<Term, Malformed<Term>>;
template const Term& mayfail_unwrap(const MayFail<Term>&);
template Term& mayfail_unwrap(MayFail<Term>&);
template std::string serializeErr(MayFail<Term>);
#endif

#ifdef WORD_CPP
template class Malformed<Word>;
template class std::expected<Word, Malformed<Word>>;
template MayFail<ProgramWord> mayfail_cast(MayFail<Word>);
template const Word& mayfail_unwrap(const MayFail<Word>&);
template Word& mayfail_unwrap(MayFail<Word>&);
template std::string serializeErr(MayFail<Word>);
#endif

////////////////////////////////////////////////////////////////

#ifdef ATOM_CPP
template class Malformed<Atom>;
template class std::expected<Atom, Malformed<Atom>>;
template MayFail<Word> mayfail_cast(MayFail<Atom>);
template MayFail<Word> mayfail_convert(MayFail<Atom>);
template const Atom& mayfail_unwrap(const MayFail<Atom>&);
template Atom& mayfail_unwrap(MayFail<Atom>&);
template std::string serializeErr(MayFail<Atom>);
#endif

#ifdef PARENTHESES_GROUP_CPP
template class Malformed<ParenthesesGroup>;
template class std::expected<ParenthesesGroup, Malformed<ParenthesesGroup>>;
template MayFail<Word> mayfail_cast(MayFail<ParenthesesGroup>);
template MayFail<Word> mayfail_convert(MayFail<ParenthesesGroup>);
template const ParenthesesGroup& mayfail_unwrap(const MayFail<ParenthesesGroup>&);
template ParenthesesGroup& mayfail_unwrap(MayFail<ParenthesesGroup>&);
template std::string serializeErr(MayFail<ParenthesesGroup>);
#endif

#ifdef SQUARE_BRACKETS_GROUP_CPP
template class Malformed<SquareBracketsGroup>;
template class std::expected<SquareBracketsGroup, Malformed<SquareBracketsGroup>>;
template MayFail<Word> mayfail_cast(MayFail<SquareBracketsGroup>);
template MayFail<Word> mayfail_convert(MayFail<SquareBracketsGroup>);
template const SquareBracketsGroup& mayfail_unwrap(const MayFail<SquareBracketsGroup>&);
template SquareBracketsGroup& mayfail_unwrap(MayFail<SquareBracketsGroup>&);
template std::string serializeErr(MayFail<SquareBracketsGroup>);
#endif

#ifdef CURLY_BRACKETS_GROUP_CPP
template class Malformed<CurlyBracketsGroup>;
template class std::expected<CurlyBracketsGroup, Malformed<CurlyBracketsGroup>>;
template MayFail<Word> mayfail_cast(MayFail<CurlyBracketsGroup>);
template MayFail<Word> mayfail_convert(MayFail<CurlyBracketsGroup>);
template const CurlyBracketsGroup& mayfail_unwrap(const MayFail<CurlyBracketsGroup>&);
template CurlyBracketsGroup& mayfail_unwrap(MayFail<CurlyBracketsGroup>&);
template std::string serializeErr(MayFail<CurlyBracketsGroup>);
#endif

#ifdef POSTFIX_PARENTHESES_GROUP_CPP
template class Malformed<PostfixParenthesesGroup>;
template class std::expected<PostfixParenthesesGroup, Malformed<PostfixParenthesesGroup>>;
template MayFail<Word> mayfail_cast(MayFail<PostfixParenthesesGroup>);
template MayFail<Word> mayfail_convert(MayFail<PostfixParenthesesGroup>);
template const PostfixParenthesesGroup& mayfail_unwrap(const MayFail<PostfixParenthesesGroup>&);
template PostfixParenthesesGroup& mayfail_unwrap(MayFail<PostfixParenthesesGroup>&);
template std::string serializeErr(MayFail<PostfixParenthesesGroup>);
#endif

#ifdef POSTFIX_SQUARE_BRACKETS_GROUP_CPP
template class Malformed<PostfixSquareBracketsGroup>;
template class std::expected<PostfixSquareBracketsGroup, Malformed<PostfixSquareBracketsGroup>>;
template MayFail<Word> mayfail_cast(MayFail<PostfixSquareBracketsGroup>);
template MayFail<Word> mayfail_convert(MayFail<PostfixSquareBracketsGroup>);
template const PostfixSquareBracketsGroup& mayfail_unwrap(const MayFail<PostfixSquareBracketsGroup>&);
template PostfixSquareBracketsGroup& mayfail_unwrap(MayFail<PostfixSquareBracketsGroup>&);
template std::string serializeErr(MayFail<PostfixSquareBracketsGroup>);
#endif
