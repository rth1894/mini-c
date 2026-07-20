# Mini-C Grammar

This document specifies the grammar accepted by Mini-C.

The grammar is written in Extended Backus-Naur Form (EBNF).

---

## Lexical Elements

### Keywords

```text
int
return
if
while
```

### Operators

```text
+ - * /
= == != < <= > >=
```

### Delimiters

```text
( ) { } , ;
```

---

## Program

$$
\begin{aligned}
Program&\rightarrow Function^+
\end{aligned}
$$

## Function

$$
\begin{aligned}
Function
&\rightarrow
\texttt{int}\ Identifier\
&\qquad (ParameterList?)\
&\qquad \{ Statement^* \}
\end{aligned}
$$

## ParameterList

$$
\begin{aligned}
ParameterList
&\rightarrow Parameter\ (,\ Parameter)^* \\
Parameter
&\rightarrow \texttt{int}\ Identifier
\end{aligned}
$$

## Statement

$$
\begin{aligned}
Statement
\rightarrow&
\ VariableDecl\\
|&\ Assignment\\
|&\ ReturnStmt\\
|&\ IfStmt\\
|&\ WhileStmt
\end{aligned}
$$

## Variable Declaration

$$
VariableDecl
\rightarrow
\texttt{int}\ Identifier = Expression\ ;
$$

## Assignment

$$
Assignment
\rightarrow
Identifier = Expression\ ;
$$

## Return

$$
ReturnStmt
\rightarrow
\texttt{return}\ Expression\ ;
$$

## If

$$
IfStmt
\rightarrow
\texttt{if}
(Expression)
\{
Statement^*
\}
$$

## While

$$
WhileStmt
\rightarrow
\texttt{while}
(Expression)
\{
Statement^*
\}
$$

## Expressions

$$
\begin{aligned}
Expression
&\rightarrow Comparison
\end{aligned}
$$

## Comparisions

$$
\begin{aligned}
Comparison
\rightarrow&
Term\\
|&Comparison\ (==|!=|<|<=|>|>=)\ Term
\end{aligned}
$$

## Terms

$$
\begin{aligned}
Term
\rightarrow&
Factor\\
|&Term\ (+|-)\ Factor
\end{aligned}
$$

## Factors

$$
\begin{aligned}
Factor
\rightarrow&
Primary\\
|&Factor\ (*|/)\ Primary
\end{aligned}
$$

## Primary

$$
\begin{aligned}
Primary
\rightarrow&
Integer\\
|&Identifier\\
|&CallExpr\\
|&(Expression)
\end{aligned}
$$

## Function Calls

$$
\begin{aligned}
CallExpr
\rightarrow&
Identifier(ParameterExprList?)
\end{aligned}
$$

## Arguments

$$
\begin{aligned}
ParameterExprList
\rightarrow&
Expression\\
|&Expression(,\ Expression)^*
\end{aligned}
$$
