#include <stdexcept>

#include "../include/parser.h"

Parser::Parser(const std::vector<Token>& tokens) :
    pos_(0),
    tokens_(tokens) {
}

const Token& Parser::current() const {
    return tokens_[pos_];
}

const Token& Parser::previous() const {
    return tokens_[pos_ - 1];
}

bool Parser::match(TokenType type) {
    if (current().type != type) return false;

    pos_++;
    return true;
}

ExprPtr Parser::parsePrimary() {
    if (match(TokenType::INTEGER))
        return std::make_unique<NumberExpr>(std::stoi(previous().lexeme));

    if (match(TokenType::IDENTIFIER))
        return std::make_unique<VariableExpr>(previous().lexeme);

    if (match(TokenType::LPAREN)) {
        auto expr = parseExpression();

        if (!match(TokenType::RPAREN)) throw std::runtime_error("Expected ')'");
        return expr;
    }

    throw std::runtime_error("Unexpected token in expression");
}

ExprPtr Parser::parseFactor() {
    auto left = parsePrimary();
    while ( current().type == TokenType::STAR || current().type == TokenType::SLASH) {
        std::string op = current().lexeme;
        pos_++;

        auto right = parsePrimary();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

ExprPtr Parser::parseTerm() {
    auto left = parseFactor();

    while (current().type == TokenType::PLUS || current().type == TokenType::MINUS) {
        std::string op = current().lexeme;
        pos_++;

        auto right = parseFactor();

        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

ExprPtr Parser::parseExpression() {
    return parseTerm();
}

StmtPtr Parser::parseStatement() {
    if (match(TokenType::KW_RETURN)) {
        auto expr = parseExpression();
        if (!match(TokenType::SEMICOLON)) throw std::runtime_error("Expected ';'");

        return std::make_unique<ReturnStmt>(std::move(expr));
    }

    throw std::runtime_error("Expected statement");
}
