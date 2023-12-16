#~

This declarative script is a configuration file..
..used to generate C++ files based on some user-defined entities..
..(called entries here)

header-dir: Name of header files (.h) directory

source-dir: Name of source files (.cpp) directory

to-definition-name: Process to transform an entry to a definition name (which can be later used as a symbolic constant)

to-header-filename: Process to transform an entry to an header filename

to-source-filename: Process to transform an entry to a source filename

to-test-filename: Process to transform an entry to a test filename

header-content: Content of the header file (.h)

source-content: Content of the source file (.cpp)

entries: List of all user-defined entities (one entity per line in a multiline string)

~#

header-dir:"include"
source-dir:"src"
test-dir:"src/test" #optional

to-definition-name:(entry):{
    # e.g.: `ThisIsATest` => `THIS_IS_A_TEST_H`
    entry |> split-camelcase |> map(upper) |> join("_") |> concat("_H")
}

to-header-filename:(entry):{"${entry}.h"}
to-source-filename:(entry):{"${entry}.cpp"}
to-test-filename:(entry):{"${entry}.cpp"}

header-content:```c++
#ifndef $DEFINITION-NAME
#define $DEFINITION-NAME



#endif // $DEFINITION-NAME

```

source-content:```c++

``` #optional

entries:```
Program
ProgramSentence
Term
ProgramWord
Association
ParenthesesGroup
SquareBracketsGroup
PostfixParenthesesGroup
PostfixSquareBracketsGroup
CurlyBracketsGroup
Quotation
Quoted
Atom
```
