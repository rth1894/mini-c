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
* Comparison statements (`==`, `!=`, `>=`, `<=`, `>`, `<`)
* `if` statements
* `while` loops (for and do while later)
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
int main()
{
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

```
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
AST Printer
```

---

#### TODO

- [ ] Type checking
- [ ] Implicit/explicit type conversions
- [ ] Three-Address Code (TAC) generation
- [ ] Control Flow Graph (CFG)
- [ ] Basic optimization passes
  - [ ] Constant folding
  - [ ] Constant propagation
  - [ ] Dead code elimination
  - [ ] Copy propagation
- [ ] LLVM IR backend
- [ ] x86-64 backend
