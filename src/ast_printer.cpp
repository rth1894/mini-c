#include <iostream>

#include "../include/ast_printer.h"

void ASTPrinter::printIndent(int indent) {
    for (int i = 0; i < indent; i++) std::cout << "  ";
}

void ASTPrinter::print(const ASTNode* node, int indent) {
    if (!node) return;

    if (auto program = dynamic_cast<const Program*> (node)) {
        std::cout << "Program\n";
        for (const auto& stmt : program->statements) print(stmt.get(), indent + 1);
        return;
    }

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

    if (auto decl = dynamic_cast<const VariableDecl*>(node)) {
        printIndent(indent);
        std::cout << "VariableDecl(" << decl->name << ")\n";
        print(decl->initializer.get(), indent+1);
        return;
    }

    if (auto assign = dynamic_cast<const AssignmentStmt*>(node)) {
        printIndent(indent);
        std::cout << "Assignment(" << assign->name << ")\n";
        print(assign->value.get(), indent + 1);
        return;
    }

    if (auto ifStmt = dynamic_cast<const IfStmt*>(node)) {
        printIndent(indent);
        std::cout << "If\n";
        printIndent(indent + 1);

        std::cout << "Condition\n";
        print(ifStmt->condition.get(), indent + 2);

        printIndent(indent + 1);
        std::cout << "Body\n";

        for (const auto& stmt : ifStmt->body) print (stmt.get(), indent + 2);
        return;
    }

    if (auto whileStmt = dynamic_cast<const WhileStmt*>(node)) {
        printIndent(indent);
        std::cout << "While\n";
        printIndent(indent + 1);

        std::cout << "Condition\n";
        print(whileStmt->condition.get(), indent + 2);
        printIndent(indent + 1);

        std::cout << "Body\n";
        for (const auto& stmt : whileStmt->body) print(stmt.get(), indent + 2);
        return;
    }
}
