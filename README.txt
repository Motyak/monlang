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

Each lib should have its own auto build mechanism (whether it's a Makefile or something else), we don't want to check ourselves if the lib is up-to-date because we are not supposed to know the details.

We're systematically going to recheck and potentially rebuild each lib if they are out-of-date (by questioning each lib auto build mechanism and re-running it says it's necessary). However, when the 'q' make flag is passed to our Makefile, it will not systematically return 1, it will only do so if the libs were not build at least once (even if it's out-of-date).

This behavior is configurable through environment variables.
