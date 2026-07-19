/*
int main() { return 0; }

becomes

define i32 @main() {
    entry:
      ret i32 0
}
*/

#include "../include/llvm_generator.h"

#include <sstream>
#include <stdexcept>

std::string LLVMGenerator::nextTemp() {
    return "%" + std::to_string(tempCounter_++);
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
    currentFunc_.str("");
    currentFunc_.clear();

    currentFunc_ << "define i32 @" << function->name << "() {\n";
    currentFunc_ << "\tentry:\n";
    for (auto& stmt : function->body) generateStatement(stmt.get());

    currentFunc_ << "}\n\n";
    return currentFunc_.str();
}

std::string LLVMGenerator::generateStatement(Stmt* stmt) {
    if (auto* ret = dynamic_cast<ReturnStmt*>(stmt)) {
        std::string value = generateExpression(ret->expr.get());

        currentFunc_ << "\tret i32 " << value << "\n";
        return "";
    }
    throw std::runtime_error("Unsupported statement");
}

std::string LLVMGenerator::generateExpression(Expr* expr) {
    if (auto* number = dynamic_cast<NumberExpr*>(expr)) return std::to_string(number->value);

    if (auto bin = dynamic_cast<BinaryExpr*>(expr)) {
        std::string lhs = generateExpression(bin->lhs.get());
        std::string rhs = generateExpression(bin->rhs.get());

        std::string temp = nextTemp();
        std::string op;

        if (bin->op == "+") op = "add";
        else if (bin->op == "-") op = "sub";
        else if (bin->op == "*") op = "mul";
        else if (bin->op == "/") op = "sdiv";
        else throw std::runtime_error("Unsupported operator");

        currentFunc_ << "\t" << temp << " = " << op << " i32 " << lhs << ", " << rhs << "\n";
        return temp;
    }

    throw std::runtime_error("Unsupported expression");
}
