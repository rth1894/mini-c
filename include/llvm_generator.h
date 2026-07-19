#pragma once

#include "ast.h"

#include <sstream>
#include <string>

class LLVMGenerator {
public:
  std::string generate(Program *program);

private:
  int tempCounter_ = 0;
  std::string nextTemp();

  std::ostringstream currentFunc_;

  std::string generateFunction(FunctionDecl *function);
  std::string generateStatement(Stmt *stmt);
  std::string generateExpression(Expr *expr);
};
