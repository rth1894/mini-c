#pragma once

#include <memory>
#include <stdexcept>

#include "ast.h"
#include "symbol_table.h"

class SemanticAnalyzer {
    public:
        void analyze(Program* prog);

    private:
        SymbolTable symbols_;
        void analyzeStatement(Stmt* stmt);
        void analyzeExpression(Expr* expr);
};
