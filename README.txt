=== ERROR CODE CONVENTION ===

--- Error code pattern

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

51 CurlyBracketsTerm
52 SquareBracketsTerm
53 Quotation
54 MultilineQuotation (Quotation)

99 Atom

0xy -> missing initiator
x0y -> missing continuator
xy0 -> missing terminator
xy9 -> malformed sub-element
xyz -> custom error (refer to manual)

--- Manual (specific error codes)

121 ProgramSentence can't start with a continuator (hit leading continuator)
122 ProgramSentence continuator must precede a word (hit trailing continuator)
123 ProgramSentence has wrong indentation

131 Term can't start with a continuator (leading continuator met)

412 multiline CurlyBracketsGroup expect indented/empty newline after initiator
413 multiline CurlyBracketsGroup must contain at least one sentence (encountered block with empty lines exclusively)

521 SquareBracketsTerm cannot be empty

541 MultilineQuotation expect indented/empty newline after initiator
542 MultilineQuotation line has wrong indentation

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
099 => Atom has no initiator seq

101 => Program has no continuator seq
502 => SquareBracketsTerm has no continuator seq
503 => Quotation has no continuator seq
504 => MultilineQuotation has no continuator seq
909 => Atom has no continuator seq

110 => Program has no terminator seq
130 => Term has no terminator seq
520 => Error would be catched by inner Term first
990 => Atom has no terminator seq

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

=== TWEAK IMPL W/ BUILD-TIME MACROS ===

Macros allow to tweak the IMPLEMENTATION of ONE specific compilation unit through the build command.

Rebuilding a compilation unit with a macro shouldn't require any other unit re-compilation since implementations depend on interfaces (headers) never on other implementations.

---

Enable trace on consumeXXX(..) functions:
  -> make -B obj/common.o TRACE=x
Restore default value (no trace):
  -> make -B obj/common.o TRACE=

Disable certain words in consumeWord(..) functions:
  -> make -B obj/Word.o DISABLE_WORDS=PG,SBG,CBG,
Restore default value (enable all words):
  -> make -B obj/Word.o DISABLE_WORDS=

Disable certain composed words in consumeAtom(..) functions:
  -> make -B obj/Atom.o DISABLE_POSTFIXES=SBG_IN_ATOM,PG_IN_ATOM,
Restore default value for Atom (enable all postfixes in Atom):
  -> make -B obj/Atom.o DISABLE_POSTFIXES=

Restore default value everywhere:
  -> make -B main TRACE= DISABLE_WORDS= DISABLE_POSTFIXES= DISABLE_ASSOCS= DISABLE_SPECIAL_ATOMS= DISABLE_DOLLARS_CBG=

---

A more convenient way to pass the same set of parameters to each build command is to add them in the make alias in env.sh.
e.g.:
`local EXTRA_ARGS="-j16 BUILD_LIBS_ONCE= DISABLE_WORDS=PG,SBG, DISABLE_POSTFIXES=PSBG_IN_ATOM"`

Despite having "persisted" some parameters, it will remain possible to use a parameter default value by appending `DISABLE_WORDS=` for instance into the build command (the last set values will be taken into account).
  -> make DISABLE_WORDS=PG -B Word.o DISABLE_WORDS=
     <=>
     make -B Word.o DISABLE_WORDS=

=== GENERATE INCLUDE GRAPH ===

# Edit .clang-uml config file

# Generate compile_commands.json file
bear -- make bin/test/all.elf CLANG=x
# Generate PlantUML diagrams
clang-uml # read options from .clang-uml
# Export .puml as svg image (vscode plugin)
