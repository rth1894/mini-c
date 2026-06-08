#pragma once

#include <memory>
#include <vector>
#include <string>

struct ASTNode {
    virtual ~ASTNode() = default;
};

using ASTPtr = std::unique_ptr<ASTNode>;

struct Expr : ASTNode {};

struct NumberExpr : Expr {
    int value;
    explicit NumberExpr(int v) : value(v) {}
};

struct VariableExpr : Expr {
    std::string name;
    explicit VariableExpr(std::string n) : name(std::move(n)) {}
};

struct BinaryExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    BinaryExpr(
        std::string op,
        std::unique_ptr<Expr> lhs,
        std::unique_ptr<Expr> rhs
    )
        : op(std::move(op)),
          lhs(std::move(lhs)),
          rhs(std::move(rhs)) {}
};
