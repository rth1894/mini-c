/* will now start returning strings instead of AST */
#pragma once

#include "ast.h"
#include "tac.h"

class IRGenerator {
    public:
        TACProgram generate(Program* program);
    private:
        int labelCounter_ = 0;
        int tempCounter_ = 0;

        std::string newLabel();
        std::string newTemp();

        void generateStatement(Stmt* stmt, TACProgram& program);
        std::string generateExpression(Expr* expr, TACProgram& program);
};
