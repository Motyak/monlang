
Evaluating expressions
  -> literals (numbers, booleans, char/byte, strings, lists, maps, lambdas)
    -> pour le moment on va faire juste nombres, booleans et strings
  -> parentheses (grouping)
  -> unary expressions (bool negation, number negation, function call)
  -> binary expressions (numbers sum, numbers product, ..)
  -> ternary expressions (ternary conditional operator)
  -> block expressions (block of code with a trailing expression)

Unary operators must be stuck (no space) with its operand.
  -> `if (...)` is a statement, `if(...)` is a function call (unary expression)
  -> `(-1)` is negative 1 literal number, `(- 1)` is a malformed number differentiation expression which is missing a left operand
Binary operators must have one space between it and each of its two operands.
  -> `(foo - bar)` is a number differentiation expression with two variables to resolve named `foo` and `bar` respectively, `(foo-bar)` is an expression resolved from a variable named `foo-bar`
Ternary operators must have one space between each operator/operand as well

---

Scanner:
1) Split the program to lines
2) 