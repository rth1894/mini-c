# MiniCPP

small compiler for a subset of the C programming language, in cpp.

## Current Features

### Lexical Analysis

* Tokenization of source code
* Integer literals
* Identifiers
* Keywords:

  * `int`
  * `return`
  * `if`
  * `else`
  * `while`

* Arithmetic operators:

  * `+`
  * `-`
  * `*`
  * `/`

* Assignment operator:

  * `=`

* Comparison operators:

  * `==`
  * `!=`
  * `>`
  * `>=`
  * `<`
  * `<=`

* Delimiters:

  * `(`
  * `)`
  * `{`
  * `}`
  * `;`

### Parsing

* Recursive-descent parser
* Expression parsing with operator precedence
* Parenthesized expressions
* Variable declarations
* Assignment statements
* Comparison expressions
* `if` statements
* `while` loops
* Function declarations
* Function calls
* Return statements

### Semantic Analysis

* Scoped symbol table
* Function symbol table
* Undefined variable detection
* Undefined function detection
* Function redefinition detection
* Function argument count validation
* Variable redefinition detection

### Abstract Syntax Tree (AST)

Supported nodes:

* Program root node
* Function declarations
* Number expressions
* Variable expressions
* Binary expressions
* Function call expressions
* Variable declarations
* Assignment statements
* `if` statements
* `while` loops
* Return statements

### Three-Address Code (TAC)

Generates TAC for:

* Function definitions
* Function calls
* Arithmetic expressions
* Variable declarations
* Assignment statements
* Comparison expressions
* `if` statements
* `while` loops
* Return statements

Example:

```text
function add:
t0 = a + b
return t0

function main:
param 2
param 3
t1 = call add, 2
return t1
```

### Optimizations

Implemented optimization passes:

* Constant folding
* Constant propagation
* Dead code elimination

Example:

Before:

```text
function main:
t0 = 3 * 2
t1 = 5 + t0
return t1
```

After optimization:

```text
function main:
return 11
```

### AST Visualization

Printing of generated ASTs for debugging and compiler development.

Example:

```c
int main() {
    return 5 + 3 * 9;
}
```

AST:

```text
Program
  Function(main)
    Body
      Return
        Binary(+)
          Number(5)
          Binary(*)
            Number(3)
            Number(9)
```

The parser correctly handles operator precedence:

```c
int main() {
    return (5 + 3) * 9;
}
```

AST:

```text
Program
  Function(main)
    Body
      Return
        Binary(*)
          Binary(+)
            Number(5)
            Number(3)
          Number(9)
```

## Build

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

## Run

```bash
./mini-c ../examples/test.mc
```

## Pipeline

```text
Source Code
    |
  Lexer
    |
  Tokens
    |
Recursive-Descent Parser
    |
   AST
    |
Semantic Analysis
    |
TAC Generation
    |
Optimization
    |
Backend
    |
Current: TAC Printer
Next: LLVM IR
```

---

#### TODO

- [ ] Type checking
- [ ] Implicit/explicit type conversions
- [ ] Control Flow Graph (CFG)
- [ ] Copy propagation
- [ ] LLVM IR generation
- [ ] x86-64 code generation
