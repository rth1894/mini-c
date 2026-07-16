#pragma once

#include <memory>
#include <string>
#include <vector>

struct ASTNode {
    virtual ~ASTNode() = default;
};

struct Expr : ASTNode {
    virtual ~Expr() = default;
};

struct Stmt : ASTNode {
    virtual ~Stmt() = default;
};

struct Decl : ASTNode {
    virtual ~Decl() = default;
};

using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;
using DeclPtr = std::unique_ptr<Decl>;

struct NumberExpr : Expr {
    int value;
    explicit NumberExpr(int v) : value(v) {}
};

struct VariableExpr : Expr {
    std::string name;
    explicit VariableExpr(std::string name) : name(std::move(name)) {}
};

struct BinaryExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    BinaryExpr(
        std::string op,
        ExprPtr lhs,
        ExprPtr rhs
    )
        : op(std::move(op)),
          lhs(std::move(lhs)),
          rhs(std::move(rhs)) {}
};

struct ReturnStmt : Stmt {
    ExprPtr expr;
    explicit ReturnStmt ( ExprPtr expr ) : expr(std::move(expr)) {}
};

struct Program : ASTNode {
    std::vector<DeclPtr> declarations;
};

struct FunctionDecl : Decl {
    std::string name;
    std::vector<std::string> parameters;
    std::vector<StmtPtr> body;

    FunctionDecl(std::string name, std::vector<std::string> parameters, std::vector<StmtPtr> body) :
        name (std::move(name)), parameters(std::move(parameters)), body(std::move(body)) {}
};

struct VariableDecl : Stmt {
    std::string name;
    ExprPtr initializer;

    VariableDecl( std::string name, ExprPtr initializer ) :
        name ( std::move(name)), initializer(std::move(initializer))
    {}
};

struct AssignmentStmt : Stmt {
    std::string name;
    ExprPtr value;

    AssignmentStmt(std::string name, ExprPtr value) :
        name(std::move(name)), value(std::move(value)) {}
};

// using vec because `if` can have multiple statements 
struct IfStmt: Stmt {
    ExprPtr condition;
    std::vector<StmtPtr> body;

    IfStmt(ExprPtr condition, std::vector<StmtPtr> body) :
        condition(std::move(condition)), body(std::move(body)) {}
};

struct WhileStmt: Stmt {
    ExprPtr condition;
    std::vector<StmtPtr> body;

    WhileStmt(ExprPtr condition, std::vector<StmtPtr> body) :
        condition(std::move(condition)), body(std::move(body)) {}
};
