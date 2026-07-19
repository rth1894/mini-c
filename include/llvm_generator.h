#pragma once

#include "ast.h"

#include <sstream>
#include <string>
#include <unordered_map>

class LLVMGenerator {
public:
  std::string generate(Program *program);

private:
  int tempCounter_ = 0;
  int labelCounter_ = 0;
  std::string nextTemp();
  std::string nextLabel();

  std::ostringstream currentFunc_;

  std::string generateCondition(Expr* expr);
  std::string generateExpression(Expr *expr);
  std::string generateFunction(FunctionDecl *function);
  std::string generateStatement(Stmt *stmt);

  std::unordered_map<std::string, std::string> variables_;
};
