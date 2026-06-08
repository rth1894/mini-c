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
* Return statements

### Abstract Syntax Tree (AST)

Supported nodes:

* Number expressions
* Variable expressions
* Binary expressions
* Return statements

### AST Visualization

Printing of generated ASTs for debugging and compiler development.

Example:

```c
return 5 + 3 * 9;
```

AST:

```text
Return
  Binary(+)
    Number(5)
    Binary(*)
      Number(3)
      Number(9)
```

The parser correctly handles operator precedence:

```c
return (5 + 3) * 9;
```

AST:

```text
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
  Parser
    |
   AST
    |
AST Printer
```
