#pragma once

#include <vector>

#include "token.h"
#include "ast.h"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    StmtPtr parseStatement();

private:
    const Token& current() const;
    const Token& previous() const;

    bool match(TokenType type);

    ExprPtr parseExpression();
    ExprPtr parseTerm();
    ExprPtr parseFactor();
    ExprPtr parsePrimary();

    size_t pos_;

    std::vector<Token> tokens_;
};
