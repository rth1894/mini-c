# MiniCPP

small compiler for a subset of the C programming language, in cpp.

- Educational compiler that implements a compilation pipeline
- Currently performs lexical analysis, recursive-descent parsing, semantic analysis, type checking, intermediate representation (Three-Address Code) generation, local optimizations, control-flow graph construction, and LLVM IR generation.


See the language specification in [docs/grammar.md](docs/grammar.md).

## Current Features

### Lexical Analysis

* Tokenization of source code
* Integer literals
* Identifiers
* Keywords:

  * `int`
  * `return`
  * `if`
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
* Function parameters
* Nested expressions
* Return statements

### Semantic Analysis

* Scoped symbol table
* Function symbol table
* Undefined variable detection
* Undefined function detection
* Function redefinition detection
* Function argument count validation
* Variable redefinition detection

### Type Checking

* Integer type validation
* Assignment type checking
* Return type checking
* Binary expression type checking
* Function call argument type checking

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
* Labels
* Conditional branches
* Unconditional jumps
* Return statements

### Optimizer

Implemented local optimization passes:

- Constant folding
- Local constant propagation
- Dead code elimination

> Note:
> Intentionally limited to basic linear code and is reset across control-flow boundaries to preserve correctness.

### Control Flow Graph

The compiler constructs a Control Flow Graph from generated TAC.
Implemented:

* Basic block formation
* Leader identification
* Successor computation
* Predecessor computation
* CFG visualization

### LLVM IR Generation

Generates LLVM IR for:

* Functions
* Variables
* Arithmetic expressions
* Comparisons
* Function calls
* Assignments
* Returns
* if statements

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
Type Checking
    |
TAC Generation
    |
Optimization
    |
Control Flow Graph
    |
    | -------------------> CFG Printer
    |
LLVM IR Generator
    |
output.ll
```
