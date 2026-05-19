The syntax should not be too complex. The basic keywords should be:

- var
- fun
- if
- else
- while
- for
- return
- justPrint
- print
- true
- false
- null

Example:

```
fun main() {
    var x = 5
    var y = 10
    var z = x + y
    justPrint(z)
}
```

Example:
```
fun factorial(n) {
    if (n == 0) {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}
fun main() {
    var result = factorial(5)
    justPrint(result)
}
```

Example:
```
fun main() {
    var i = 0
    while (i < 5) {
        justPrint(i)
        i = i + 1
    }
}
```
Example:
```
fun main() {
    for (var i = 0; i < 5; i = i + 1) {
        justPrint(i)
    }
}
```

and so on. Spread this concept properly across the files under concept. Add syntax-related information to each of AST.md, BUILD.md, CODEGEN.md, IR.md, LEX.md, PARSE.md, PLATFORM.md, RUNTIME.md, STRUCT.md, and SYNTAX.md. For example, AST.md should explain what the syntax tree looks like, PARSE.md should explain how the syntax is parsed, and CODEGEN.md should explain how the syntax is turned into machine code.

SYNTAX.md should describe the overall syntax structure and rules. It should explain how the syntax is written, which keywords are used, how variables are declared, and how functions are written. The other files should contain more detailed information about how the syntax is processed.