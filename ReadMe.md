# <span style="color:red">Expression Evaluater</span>

an expression evaluater that takes an expression stored in a string or a file, and evaluates it into a number, or prints an error if the expression in invalid.

## <span style="color:green">Types</span>

- numbers
- operators (+ - \* /)
- parenthesis ( )
- spaces

## <span style="color:green">Priority</span>

- parenthesis
- "\*" "/"
- "+" "-"

## <span style="color:green">Invalid Expressions</span>

- if it contains a letter that is not part of the types above
- if the expression is empty, (or an expression inside parenthesis is empty)
- wrong parenthesis order ( ), or uncomplete pairs
- syntax errors like (1 ++ 2), (1 +), (\* 1)

## <span style="color:green">Examples</span>

### Input:

(code.txt)

```
(5 + 5) * 2 + 3 * (5 + 1)
```

### Output:

```bash command-line
38
```

### Input:

(code.txt)

```
123
(1) + 1
2 + (3)
10 + (20
20 * 3
```

### Output:

```bash command-line
123
2
5
Error: invalid expression
```

Note: the program will stop after an error accures and will not continue
