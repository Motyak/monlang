
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

---

("123 321":[1, 2, 3], 321) "abc:def"
  -> sentence 1:
    -> word 1: ParenthesesGroup: `("123 321":[1, 2, 3])`
      -> word 1: Association: `"123 321":[1, 2, 3]`
        -> left part: Quotation: `"123 321"`
        -> right part: SquareBracketsGroup: `[1, 2, 3]`
          -> word 1: Atom: `1`
          -> word 2: Atom: `2`
          -> word 3: Atom: `3`
      -> word 2: Atom: `321`
    -> word 2: Quotation: `"abc:def"`


("123 321":[1, 2, 3], 321 fds
  -> ce corpus pose un problème car on rencontre un SPACE après le 321, et on s'attendait à avoir une ',' avant de recontrer un SPACE ou un NEWLINE



("123 321":[1, 2, 3], 
  -> ce corpus pose un problème car on rencontre un NEWLINE là où on essaie de parser un ProgramWord



("123 321":[1, 2, 3], 321) "abc:def"
  -> Dans ce cas exemple, à supposé que j'essaie de parser une Association en premier, comment pourrais-je savoir si il faut que j'interprète le premier ProgramWord de la ProgramSentence comme une Association ou bien une ParenthesesGroup ??
    -> Il faut être capable de déduire si le ':' est enprisonné ou pas (c'est le cas ici, du coup on va interpréter le mot comme un ParenthesesGroup QUI CONTIENT une Association, plutôt qu'interprété comme une Association mal formée.


("123 321"):(fds)
  -> en parsant le premier caractère, on pourrait croire que le mot est un ParenthesesGroup, alors qu'en réalité c'est une Association avec en firstPart un ParenthesesGroup.
