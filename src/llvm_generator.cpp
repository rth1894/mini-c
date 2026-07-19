/*
int main() { return 0; }

becomes

define i32 @main() {
entry:
      ret i32 0
}
*/

#include "../include/llvm_generator.h"

#include <cstddef>
#include <memory>
#include <sstream>
#include <stdexcept>

std::string LLVMGenerator::nextTemp() {
    return "%" + std::to_string(tempCounter_++);
}

std::string LLVMGenerator::nextLabel() {
    return "L" + std::to_string(labelCounter_++);
}

std::string LLVMGenerator::generate(Program *program) {
  std::ostringstream out;
  for (auto &decl : program->declarations) {
    auto *function = dynamic_cast<FunctionDecl *>(decl.get());

    if (function) out << generateFunction(function);
  }
  return out.str();
}

std::string LLVMGenerator::generateFunction(FunctionDecl* function) {
    tempCounter_ = 0;
    labelCounter_ = 0;
    variables_.clear();
    currentFunc_.str("");
    currentFunc_.clear();

    // start
    currentFunc_ << "define i32 @" << function->name << "(";
    // parameters
    for (std::size_t i=0; i<function->parameters.size(); i++) {
        if (i) currentFunc_ << ", ";
        currentFunc_ << "i32 %" << function->parameters[i];
    }
    currentFunc_ << ") {\n";

    currentFunc_ << "entry:\n";

    for (const auto& param : function->parameters) {
        std::string slot = "%" + param + ".addr";

        variables_[param] = slot;

        currentFunc_
            << "\t" << slot << " = alloca i32\n"
            << "\t" << "store i32 %" << param << ", ptr " << slot << "\n\n";
    }

    // body
    for (auto& stmt : function->body) generateStatement(stmt.get());

    currentFunc_ << "}\n\n";
    return currentFunc_.str();
}

std::string LLVMGenerator::generateStatement(Stmt* stmt) {
    if (auto* decl = dynamic_cast<VariableDecl*>(stmt)) {
        std::string slot = "%" + decl->name;
        variables_[decl->name] = slot;

        currentFunc_ << "\t" << slot << " = alloca i32\n";

        std::string value = generateExpression(decl->initializer.get());

        currentFunc_ << "\tstore i32 " << value << ", ptr " << slot << "\n\n";
        return "";
    }

    if (auto* assign = dynamic_cast<AssignmentStmt*>(stmt)) {
        auto it = variables_.find(assign->name);
        if (it == variables_.end()) throw std::runtime_error("Unknown variable '" + assign->name+ "'");

        std::string value = generateExpression(assign->value.get());

        currentFunc_ << "\tstore i32 " << value << ", ptr " << it->second << "\n";
        return "";
    }

    if (auto* ifStmt = dynamic_cast<IfStmt*>(stmt)) {
        std::string condition = generateCondition(ifStmt->condition.get());
        std::string thenLabel = nextLabel();
        std::string endLabel = nextLabel();

        currentFunc_ << "\tbr i1 " << condition << ", label %" << thenLabel << ", label %" << endLabel << "\n\n";

        currentFunc_ << thenLabel << ":\n";
        for (auto& statement : ifStmt->body) generateStatement(statement.get());

        currentFunc_ << "\tbr label %" << endLabel << "\n\n";
        currentFunc_ << endLabel << ":\n";
        return "";
    }

    if (auto* whileStmt = dynamic_cast<WhileStmt*>(stmt)) {
        std::string conditionLabel = nextLabel();
        std::string bodyLabel = nextLabel();
        std::string endLabel = nextLabel();

        currentFunc_ << "\tbr label %" << conditionLabel << "\n\n";
        currentFunc_ << conditionLabel << ":\n";

        std::string condition = generateCondition(whileStmt->condition.get());
        currentFunc_ << "\tbr i1 " << condition << ", label %" << bodyLabel << ", label %" << endLabel << "\n\n";

        currentFunc_ << bodyLabel << ":\n";

        for (auto& stmt : whileStmt->body) {
            generateStatement(stmt.get());
        }

        currentFunc_ << "\tbr label %" << conditionLabel << "\n\n";
        currentFunc_ << endLabel << ":\n";
        return "";
    }

    if (auto* ret = dynamic_cast<ReturnStmt*>(stmt)) {
        std::string value = generateExpression(ret->expr.get());

        currentFunc_ << "\n\tret i32 " << value << "\n";
        return "";
    }
    throw std::runtime_error("Unsupported statement");
}

std::string LLVMGenerator::generateExpression(Expr* expr) {
    if (auto* number = dynamic_cast<NumberExpr*>(expr)) return std::to_string(number->value);

    if (auto* var = dynamic_cast<VariableExpr*>(expr)) {
        auto it = variables_.find(var->name);

        if (it == variables_.end()) throw std::runtime_error("Unknown variable '" + var->name + "'");

        std::string temp = nextTemp();
        currentFunc_ << "\t" << temp << " = load i32, ptr " << it->second << "\n";
        return temp;
    }

    if (auto* call = dynamic_cast<CallExpr*>(expr)) {
        std::vector<std::string> args;

        for (auto& arg : call->arguments) args.push_back(generateExpression(arg.get()));

        std::string temp = nextTemp();

        currentFunc_ << "\t" << temp << " = call i32 @" << call->who << "(";
        for (std::size_t i=0; i<args.size(); i++) {
            if (i) currentFunc_ << ", ";
            currentFunc_ << "i32 " << args[i];
        }
        currentFunc_ << ")\n";
        return temp;
    }

    if (auto bin = dynamic_cast<BinaryExpr*>(expr)) {
        std::string lhs = generateExpression(bin->lhs.get());
        std::string rhs = generateExpression(bin->rhs.get());

        if (bin->op == "+" || bin->op == "-" || bin->op == "*" || bin->op == "/") {
            std::string temp = nextTemp();
            std::string op;

            if (bin->op == "+") op = "add";
            else if (bin->op == "-") op = "sub";
            else if (bin->op == "*") op = "mul";
            else if (bin->op == "/") op = "sdiv";

            currentFunc_ << "\t" << temp << " = " << op << " i32 " << lhs << ", " << rhs << "\n";
            return temp;
        }

        std::string predicate;

        if (bin->op == "==") predicate = "eq";
        else if (bin->op == "!=") predicate = "ne";
        else if (bin->op == ">") predicate = "sgt";
        else if (bin->op == ">=") predicate = "sge";
        else if (bin->op == "<") predicate = "slt";
        else if (bin->op == "<=") predicate = "sle";
        else throw std::runtime_error("Unsupported expression");

        std::string cmp = generateCondition(expr);
        std::string result = nextTemp();
        currentFunc_ << "\t" << result << " = zext i1 " << cmp << " to i32\n";
        return result;
    }
    return "";
}

std::string LLVMGenerator::generateCondition(Expr* expr) {
    auto* bin = dynamic_cast<BinaryExpr*>(expr);

    if (!bin) throw std::runtime_error("Expected comparison in condition");

    std::string lhs = generateExpression(bin->lhs.get());
    std::string rhs = generateExpression(bin->rhs.get());

    std::string predicate;

    if (bin->op == "==") predicate = "eq";
    else if (bin->op == "!=") predicate = "ne";
    else if (bin->op == ">") predicate = "sgt";
    else if (bin->op == ">=") predicate = "sge";
    else if (bin->op == "<") predicate = "slt";
    else if (bin->op == "<=") predicate = "sle";
    else throw std::runtime_error("Expected comparison operator");

    std::string temp = nextTemp();
    currentFunc_ << "\t" << temp << " = icmp " << predicate << " i32 " << lhs << ", " << rhs << "\n";

    return temp;
}
