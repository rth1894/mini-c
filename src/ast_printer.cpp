#include <iostream>

#include "../include/ast_printer.h"

void ASTPrinter::printIndent(int indent) {
    for (int i = 0; i < indent; i++) std::cout << "  ";
}

void ASTPrinter::print(const ASTNode* node, int indent) {
    if (!node) return;

    if (auto num = dynamic_cast<const NumberExpr*>(node)) {
        printIndent(indent);
        std::cout << "Number(" << num->value << ")\n";
        return;
    }

    if (auto var = dynamic_cast<const VariableExpr*>(node)) {
        printIndent(indent);

        std::cout << "Variable(" << var->name << ")\n";
        return;
    }

    if (auto bin = dynamic_cast<const BinaryExpr*>(node)) {
        printIndent(indent);
        std::cout << "Binary(" << bin->op << ")\n";

        print(bin->lhs.get(), indent + 1);
        print(bin->rhs.get(), indent + 1);

        return;
    }

    if (auto ret = dynamic_cast<const ReturnStmt*>(node)) {
        printIndent(indent);
        std::cout << "Return\n";
        print( ret->expr.get(), indent + 1);
        return;
    }
}
