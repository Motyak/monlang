=== ERROR CODE CONVENTION ===

--- ERROR CODE PATTERN

0xy -> missing initiator
x0y -> missing continuator
xy0 -> missing terminator
xy9 -> malformed sub-element
xyz -> custom error (refer to manual)

---

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

--- ERRORS THAT DON'T EXIST, WITH JUSTIFICATION

011 => Program has no initiator seq
012 => ProgramSentence has no initiator seq

101 => Program has no continuator seq

110 => Program has no terminator seq

--- ERRORS THAT CANT HAPPEN FROM A PROGRAM CONSUMPTION CONTEXT, WITH JUSTIFICATION

125 ProgramSentence cannot be empty (hit early EOF)
      => `consumeProgram` checks for EOF before calling `consumeProgramSentence`
995 Atom cannot be empty (hit early EOF)
      => `consumeProgram` checks for EOF way before...

--- MANUAL (SPECIFIC ERROR CODES)

121 ProgramSentence can't start with a continuator (hit leading continuator)
122 ProgramSentence continuator must precede a word (hit trailing continuator)
123 ProgramSentence has wrong indentation

131 Term can't start with a continuator (leading continuator met)
135 EOF encountered in Term

412 multiline CurlyBracketsGroup must contain at least one sentence (hit terminator right after initiator)
413 multiline CurlyBracketsGroup must contain at least one sentence (block with empty lines exclusively)

421 ParenthesesGroup can't start with a continuator (leading continuator met)
422 ParenthesesGroup continuator must precede a word (trailing continuator met)

431 SquareBracketsGroup can't start with a continuator (leading continuator met)
432 SquareBracketsGroup continuator must precede a word (trailing continuator met)

992 Atom cannot be empty (hit a reserved character)

=== MAKEFILE DESIGN ===

Support :
- parallel mode (-j --jobs)
- question mode (-q --question)
- dry mode (-n --dryrun)

Each lib should have its own build system (whether it's a Makefile or something else), we don't want to check ourselves if a lib is up-to-date, we want to use its build system instead.
By default the libs are built only once, even if their code changes afterward.
You can toggle a variable in order to track libs code change, by doing so we're systematically going to recheck and potentially rebuild each lib if they are out-of-date (by questioning its build system and re-running it if it says it's necessary).

=== GENERATE INCLUDE GRAPH ===

# Edit .clang-uml config file

# Generate compile_commands.json file
bear -- make bin/test/all.elf CLANG=y
# Generate PlantUML diagrams
clang-uml # read options from .clang-uml
# Export .puml as svg image (vscode plugin)
