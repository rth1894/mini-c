#pragma once

#include "ast.h"

class ASTPrinter {
public:
    void print(const ASTNode* node, int indent = 0);

private:
    void printIndent(int indent);
};
