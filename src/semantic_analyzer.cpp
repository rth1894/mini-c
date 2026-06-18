#include "../include/semantic_analyzer.h"

#include <iostream>

void SemanticAnalyzer::analyze(Program* prog) {
    for (auto& stmt : prog->statements)
        analyzeStatement(stmt.get());
}

void SemanticAnalyzer::analyzeStatement(Stmt* stmt) {
    if (auto decl = dynamic_cast<VariableDecl*>(stmt)) {
        if (!decl->initializer)
            throw std::runtime_error("Variable must be initialized");

        analyzeExpression(decl->initializer.get());

        if (!symbols_.declare(decl->name))
            throw std::runtime_error("Redefinition of variable '" + decl->name + "'");
        return;
    }

    if (auto ret = dynamic_cast<ReturnStmt*>(stmt)) {
        analyzeExpression(ret->expr.get());
        return;
    }

    if (auto assign = dynamic_cast<AssignmentStmt*>(stmt)) {
        if (!symbols_.exist(assign->name))
            throw std::runtime_error("Undefined variable '" + assign->name + "'");

        analyzeExpression(assign->value.get());
        return;
    }
}

void SemanticAnalyzer::analyzeExpression(Expr* expr) {
    if (dynamic_cast<NumberExpr*>(expr)) return;

    if (auto var = dynamic_cast<VariableExpr*>(expr)) {
        if (!symbols_.exist(var->name))
            throw std::runtime_error("Undefined variable '" + var->name + "'");
        return;
    }

    if (auto bin = dynamic_cast<BinaryExpr*>(expr)) {
        analyzeExpression(bin->lhs.get());
        analyzeExpression(bin->rhs.get());
        return;
    }
}
