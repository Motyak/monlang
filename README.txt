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


=== MAKEFILE DESIGN ===

Each lib should have its own build system (whether it's a Makefile or something else), we don't want to check ourselves if a lib is up-to-date, we want to use its build system instead.

By default the libs are built only once, even if their code changes afterward.
You can toggle a variable in order to track libs code changes, by doing so we're systematically going to recheck and potentially rebuild each lib if they are out-of-date (by questioning each lib build system and re-running it if it says it's necessary).
However, keep in mind that when answering the 'q' make flag, our Makefile will only be able to detect if a lib is missing, not if it's out-of-date.
