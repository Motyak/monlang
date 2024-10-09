=== ERROR CODE CONVENTION ===

--- Error code pattern

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

--- Manual (specific error codes)

121 ProgramSentence can't start with a continuator (hit leading continuator)
122 ProgramSentence continuator must precede a word (hit trailing continuator)
123 ProgramSentence has wrong indentation

131 Term can't start with a continuator (leading continuator met)

412 multiline CurlyBracketsGroup must contain at least one sentence (hit terminator right after initiator)
413 multiline CurlyBracketsGroup must contain at least one sentence (block with empty lines exclusively)

992 Atom cannot be empty (hit a reserved character)

--- Errors that can't happen from a program consumption context, with justification

125 ProgramSentence cannot be empty (hit early EOF)
      => `consumeProgram` checks for EOF before calling `consumeProgramSentence`
135 Term cannot be empty (hit early EOF)
      => in all "group" entities comprising of terms, we check for EOF before loop first iteration
995 Atom cannot be empty (hit early EOF)
      => `consumeProgram` checks for EOF way before...

--- Errors that don't exist, with justification

011 => Program has no initiator seq
012 => ProgramSentence has no initiator seq
013 => Term has no initiator seq

101 => Program has no continuator seq

110 => Program has no terminator seq
130 => Term has no terminator seq

=== MAKEFILE DESIGN ===

Support :
- parallel mode (-j --jobs)
- question mode (-q --question)
- dry mode (-n --dry-run)

Each lib should have its own build system (whether it's a Makefile or something else), we don't want to check ourselves if a lib is up-to-date, we want to use its build system instead.
By default the libs are built only once, even if their code changes afterward.
You can toggle a variable in order to track libs code change, by doing so we're systematically going to recheck and potentially rebuild each lib if they are out-of-date (by questioning its build system and re-running it if it says it's necessary).

When parallel mode enabled, having multiple "goal" targets in the make command will run the goals in parallel rather than sequentially (which is probably not what you expect).
If you want to run multiple goals in a row, use one make command per goal (you can even chain commands with `&&`).

=== GENERATE INCLUDE GRAPH ===

# Edit .clang-uml config file

# Generate compile_commands.json file
bear -- make bin/test/all.elf CLANG=y
# Generate PlantUML diagrams
clang-uml # read options from .clang-uml
# Export .puml as svg image (vscode plugin)
