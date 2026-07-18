/* Only has Int for now */
#include "../include/type_checker.h"

#include <stdexcept>

void TypeChecker::check(Program* program) {
    for (auto& decl : program->declarations) {
        auto* func = dynamic_cast<FunctionDecl*>(decl.get());
        if (func) checkFunction(func);
    }
}

void TypeChecker::checkFunction(FunctionDecl* function) {
    for (auto& stmt : function->body) checkStatement(stmt.get());
}

void TypeChecker::checkStatement(Stmt* stmt) {
    if(auto var = dynamic_cast<VariableDecl*>(stmt)) {
        checkExpression(var->initializer.get());
        return;
    }

    if(auto assign = dynamic_cast<AssignmentStmt*>(stmt)) {
        checkExpression(assign->value.get());
        return;
    }

    if (auto ret = dynamic_cast<ReturnStmt*>(stmt)) {
        checkExpression(ret->expr.get());
        return;
    }

    if (auto ifStmt = dynamic_cast<IfStmt*>(stmt)) {
        checkExpression(ifStmt->condition.get());

        for (auto& s : ifStmt->body) checkStatement(s.get());
        return;
    }

    if (auto whileStmt = dynamic_cast<WhileStmt*>(stmt)) {
        checkExpression(whileStmt->condition.get());

        for (auto& s : whileStmt->body) checkStatement(s.get());
        return;
    }
}

Type TypeChecker::checkExpression(Expr* expr) {
    if (dynamic_cast<NumberExpr*>(expr) || dynamic_cast<VariableExpr*>(expr)) return Type::Int;

    if (auto bin = dynamic_cast<BinaryExpr*>(expr)) {
        Type lhs = checkExpression(bin->lhs.get());
        Type rhs = checkExpression(bin->rhs.get());

        if (lhs != rhs) throw std::runtime_error("Type mismatch in binary expression");
        return lhs;
    }

    if (auto call = dynamic_cast<CallExpr*>(expr)) {
        for (auto& arg : call->arguments) checkExpression(arg.get());
        return Type::Int;
    }
    throw std::runtime_error("Unknown expression");
}
