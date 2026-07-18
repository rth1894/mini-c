#pragma once

#include "ast.h"
#include "type.h"

class TypeChecker {
    public:
        void check(Program* program);
    private:
        void checkFunction(FunctionDecl* function);
        void checkStatement(Stmt* stmt);
        Type checkExpression(Expr* expr);
};
