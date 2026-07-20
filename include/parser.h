#pragma once

#include <vector>

#include "token.h"
#include "ast.h"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    StmtPtr parseStatement();

    std::unique_ptr<Program> parseProgram();

private:
    [[noreturn]] void error(const std::string& message);

    const Token& current() const;
    const Token& previous() const;
    const Token& peek() const;

    bool match(TokenType type);

    ExprPtr parseExpression();
    ExprPtr parseComparison();
    ExprPtr parseTerm();
    ExprPtr parseFactor();
    ExprPtr parsePrimary();

    StmtPtr parseVariableDeclaration();
    StmtPtr parseAssignmentStatement();
    StmtPtr parseIfStatement();
    StmtPtr parseWhileStatement();

    DeclPtr parseFunction();

    size_t pos_;

    std::vector<Token> tokens_;
};
