
```
let reduce (reducer, init, list):{
    var acc init
    foreach list {
        acc = reducer(acc, $1)
    }
    acc
}

let sum (x, y):{x + y}
reduce(sum, 0, [1, 2, 3]) # 6
reduce(sum, 'T', "ommy") # Tommy
```


```
let make-account (balance):{
    var _balance (balance) # create state variable from arg value

    var withdraw (amount):{
        _balance -= amount
        _balance
    }

    var deposit (amount):{
        _balance += amount
        _balance
    }

    (msg, args...):{
        let dispatch ["withdraw":withdraw, "deposit":deposit]
        dispatch[msg](args)
    }
}

var account make-account(100)
let acc-withdraw(x) account("withdraw", x)
let acc-deposit(x) account("deposit", x)

acc-withdraw(25) # 75
acc-withdraw(25) # 50
acc-deposit(10) # 60
acc-deposit(10) # 70
```

---

```
/* parse program */
variable sentences[]
while (input non vide) {
    parse program sentence
    parse newline
    ajout de la sentence dans $sentences
}

/* parse program sentence */
variable words[]
parse program word
ajout du program word à $words
while (input non vide) {
    parse space
    parse program word
    ajout du program word à $words
}

/* parse program word */

```
---

```
"Hello, world!"
```
=>
program sentence 1:
  -> program word 1: `"Hello, world!"`
    -> atom: `Hello, world!`

```
((2 + 3) * 5)
```
=>
program sentence 1:
  -> program word 1: `((2 + 3) * 5)`
    -> parentheses group word 1: `(2 + 3)`
      -> parentheses group word 1: `2`
      -> parentheses group word 2: `+`
      -> parentheses group word 3: `3`
    -> parentheses group word 2: `*`
    -> parentheses group word 3: `5`

```
["key1":"val1", "key2":"val2"]
```
=>
program sentence 1:
  -> program word 1: `["key1":"val1", "key2":"val2"]`
    -> square brackets group word 1: `"key1":"val1"`
      -> association left part: `"key1"`
      -> association right part: `"val1"`
    -> square brackets group word 2: `"key2":"val2"`
      -> association left part: `"key2"`
      -> association right part: `"val2"`

```
let sum (x, y):{x + y}
```
=>
program sentence 1: 
  -> program word 1: `let` (atom)
  -> program word 2: `sum`
  -> program word 3: `(x, y):{x + y}`
    -> association left part: `(x, y)`
      -> word1: `x`
      -> word2: `y`
    -> association right part: `{x + y}`
      -> program sentence: `x + y`
        -> program word 1: `x`
        -> program word 2: `+`
        -> program word 3: `y`


```
reduce(sum, 0, [1, 2, 3])
```
=>
program sentence 1:
  -> program word 1: `reduce(sum, 0, [1, 2, 3])`
    -> atom
// note: at this point we don't distinguish yet a unary expression from any other word type (language keyword, variable, primitive expression, .....)