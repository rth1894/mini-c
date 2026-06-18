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

using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;

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
    std::vector<StmtPtr> statements;
};

struct VariableDecl : Stmt {
    std::string name;
    ExprPtr initializer;

    VariableDecl( std::string name, ExprPtr initializer ) :
        name ( std::move(name)), initializer(std::move(initializer))
    {
    }
};

struct AssignmentStmt : Stmt {
    std::string name;
    ExprPtr value;

    AssignmentStmt(std::string name, ExprPtr value) :
        name(std::move(name)), value(std::move(value)) {}
};
