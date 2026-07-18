#include "../include/ir_generator.h"

#include <stdexcept>

std::string IRGenerator::newTemp() {
    return "t" + std::to_string(tempCounter_++);
}

std::string IRGenerator::newLabel() {
    return "L" + std::to_string(labelCounter_++);
}

TACProgram IRGenerator::generate(Program* program) {
    TACProgram ir;
    for (auto& decl : program->declarations) {
        auto* func = dynamic_cast<FunctionDecl*>(decl.get());
        if(!func) continue;

        ir.push_back({"function", "", func->name, ""});

        for (auto& stmt : func->body) generateStatement(stmt.get(), ir);
    }
    return ir;
}

void IRGenerator::generateStatement(Stmt* stmt, TACProgram& program) {
    if (auto decl = dynamic_cast<VariableDecl*>(stmt)) {
        std::string value = generateExpression(decl->initializer.get(), program);

        program.push_back({"=", decl->name, value, ""});
        return;
    }
    if (auto assign = dynamic_cast<AssignmentStmt*>(stmt)) {
        std::string value = generateExpression(assign->value.get(), program);

        program.push_back({"=", assign->name, value, ""});
        return;
    }
    if (auto ifStmt = dynamic_cast<IfStmt*>(stmt)) {
        std::string condition = generateExpression(ifStmt->condition.get(), program);
        std::string endLabel = newLabel();

        program.push_back({"ifFalse", "", condition, endLabel});

        for (auto& statement : ifStmt->body)
            generateStatement(statement.get(), program);

        program.push_back({"label", "", endLabel, ""});
        return;
    }
    if (auto whileStmt = dynamic_cast<WhileStmt*>(stmt)) {
        std::string startLabel = newLabel();
        std::string endLabel = newLabel();

        program.push_back({"label", "", startLabel, ""});
        std::string condition = generateExpression(whileStmt->condition.get(), program);
        program.push_back({"ifFalse", "", condition, endLabel});

        for (auto& statement : whileStmt->body)
            generateStatement(statement.get(), program);

        program.push_back({"goto", "", startLabel, ""});
        program.push_back({"label", "", endLabel, ""});

        return;
    }
    if (auto ret = dynamic_cast<ReturnStmt*>(stmt)) {
        std::string value = generateExpression(ret->expr.get(), program);

        program.push_back({"return", "", value, ""});
        return;
    }
    throw std::runtime_error("Unsupported statement");
}

std::string IRGenerator::generateExpression(Expr* expr, TACProgram& program) {
    if (auto num = dynamic_cast<NumberExpr*>(expr)) return std::to_string(num->value);
    if (auto var = dynamic_cast<VariableExpr*>(expr)) return var->name;

    if (auto bin = dynamic_cast<BinaryExpr*>(expr)) {
        std::string lhs = generateExpression(bin->lhs.get(), program);
        std::string rhs = generateExpression(bin->rhs.get(), program);

        std::string temp = newTemp();

        program.push_back({bin->op, temp, lhs, rhs});
        return temp;
    }

    if (auto call = dynamic_cast<CallExpr*>(expr)) {
        for (auto& arg : call->arguments) {
            std::string value = generateExpression(arg.get(), program);
            program.push_back({"param", "", value, ""});
        }
        std::string temp = newTemp();
        program.push_back({"call", temp, call->who, std::to_string(call->arguments.size())});
        return temp;
    }

    throw std::runtime_error("Unsupported expression");
}

/*
a + b * 3
  >generate(a + b * 3)
  > generate(a) -> "a"
  > generate(b * 3) -> generate(b) -> "b" -> generate(3) -> "3" -> t0 = b * 3
  > t1 = a + t0
*/
