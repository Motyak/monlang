=== ERROR CODE CONVENTION ===

11 Program
12 ProgramSentence
13 Term
14 ProgramWord
15 Word

21 Association
22 Path

31 PostfixParenthesesGroup
32 PostfixSquareBracketsGroup

41 CurlyBracketsGroup
42 ParenthesesGroup
43 SquareBracketsGroup

51 SquareBracketsTerm
52 Quotation

99 Atom

---

0xy -> missing initiator
x0y -> missing continuator
xy0 -> missing terminator
xy9 -> malformed sub-element
otherwise -> custom error (refer to manual)

---

121 ProgramSentence cannot be empty (leading terminator met)
122 ProgramSentence continuator must precede a word (trailing continuator met)
