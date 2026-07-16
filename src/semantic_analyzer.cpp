#include "../include/semantic_analyzer.h"

#include <iostream>

void SemanticAnalyzer::analyze(Program* prog) {
    // collect func signature
    for (auto& decl : prog->declarations) {
        auto* func = dynamic_cast<FunctionDecl*>(decl.get());

        if (!func) continue;

        if (!symbols_.declareFunction(func->name, func->parameters.size()))
            throw std::runtime_error("Redefinition of function '" + func->name + "'");
    }

    // analyze every func body
    for (auto& decl : prog->declarations) {
        auto* func = dynamic_cast<FunctionDecl*>(decl.get());
        if (func) analyzeFunction(func);
    }
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

    if (auto ifStmt = dynamic_cast<IfStmt*>(stmt)) {
        analyzeExpression(ifStmt->condition.get());
        symbols_.pushScope();

        for (auto& statement : ifStmt->body) analyzeStatement(statement.get());
        symbols_.popScope();
        return;
    }

    if (auto whileStmt = dynamic_cast<WhileStmt*>(stmt)) {
        analyzeExpression(whileStmt->condition.get());
        symbols_.pushScope();

        for (auto& statement : whileStmt-> body) analyzeStatement(statement.get());
        symbols_.popScope();
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

    if (auto call = dynamic_cast<CallExpr*>(expr)) {
        if (!symbols_.functionExists(call->who)) throw std::runtime_error("Undefined function '" + call->who + "'");

        if (call->arguments.size() != symbols_.parameterCount(call->who))
            throw std::runtime_error("Incorrect number of args in call to '" + call->who + "'");

        for (auto& arg : call ->arguments) analyzeExpression(arg.get());
        return;
    }
}

void SemanticAnalyzer::analyzeFunction(FunctionDecl* func) {
    symbols_.pushScope();
    for (const auto& param : func->parameters)
        if (!symbols_.declare(param)) throw std::runtime_error("Duplicate parameter '" + param + "'");

    for (auto& stmt : func->body) analyzeStatement(stmt.get());
    symbols_.popScope();
}
