
input:
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
