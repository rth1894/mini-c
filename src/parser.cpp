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

const Token& Parser::peek() const {
    return tokens_[pos_ + 1];
}

bool Parser::match(TokenType type) {
    if (current().type != type) return false;

    pos_++;
    return true;
}

ExprPtr Parser::parsePrimary() {
    if (match(TokenType::INTEGER))
        return std::make_unique<NumberExpr>(std::stoi(previous().lexeme));

    if (match(TokenType::IDENTIFIER)) {
        std::string name = previous().lexeme;

        if (match(TokenType::LPAREN)) {
            std::vector<ExprPtr> arguments;

            if (current().type != TokenType::RPAREN) {
                do {
                    arguments.push_back(parseExpression());
                }
                while(match(TokenType::COMMA));
            }

            if (!match(TokenType::RPAREN)) throw std::runtime_error("Expected ')'");

            return std::make_unique<CallExpr>(name, std::move(arguments));
        }
        // is variable
        return std::make_unique<VariableExpr>(name);
    }

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
    return parseComparison();
}

ExprPtr Parser::parseComparison() {
    auto left = parseTerm();

    while (current().type == TokenType::EQ ||
           current().type == TokenType::NEQ ||
           current().type == TokenType::LT ||
           current().type == TokenType::LE ||
           current().type == TokenType::GT ||
           current().type == TokenType::GE
      ) {
        std::string op = current().lexeme;
        pos_++;

        auto right = parseTerm();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

StmtPtr Parser::parseStatement() {
    if (current().type == TokenType::KW_INT) return parseVariableDeclaration();

    if (current().type == TokenType::IDENTIFIER && pos_ + 1 < tokens_.size() && tokens_[pos_ + 1].type == TokenType::ASSIGN)
        return parseAssignmentStatement();

    if (match(TokenType::KW_RETURN)) {
        auto expr = parseExpression();

        if (!match(TokenType::SEMICOLON)) throw std::runtime_error("Expected ';'");

        return std::make_unique<ReturnStmt>(std::move(expr));
    }

    if (current().type == TokenType::KW_IF) return parseIfStatement();

    if (current().type == TokenType::KW_WHILE) return parseWhileStatement();

    throw std::runtime_error("Unknown statement");
}

StmtPtr Parser::parseVariableDeclaration() {
    if (!match(TokenType::KW_INT)) return nullptr;
    if (!match(TokenType::IDENTIFIER)) throw std::runtime_error("Expected variable name");

    std::string name = previous().lexeme;

    if (!match(TokenType::ASSIGN)) throw std::runtime_error("Expected '='");
    auto init = parseExpression();

    if (!match(TokenType::SEMICOLON)) throw std::runtime_error("Expected ';'");

    return std::make_unique<VariableDecl>( name, std::move(init) );
}

std::unique_ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();

    while (current().type != TokenType::END)
        program->declarations.push_back(parseFunction());

    return program;
}

StmtPtr Parser::parseAssignmentStatement() {
    if (!match(TokenType::IDENTIFIER))
        throw std::runtime_error("Expected variable name");

    std::string name = previous().lexeme;

    if (!match(TokenType::ASSIGN))
        throw std::runtime_error("Expected '='");

    auto value = parseExpression();

    if (!match(TokenType::SEMICOLON))
        throw std::runtime_error("Expected ';'");

    return std::make_unique<AssignmentStmt>(name, std::move(value));
}

StmtPtr Parser::parseIfStatement() {
    if (!match(TokenType::KW_IF)) throw std::runtime_error("Expected 'if'");
    if (!match(TokenType::LPAREN)) throw std::runtime_error("Expected '('");
    
    auto condition = parseExpression();

    if (!match(TokenType::RPAREN)) throw std::runtime_error("Expected ')'");
    if (!match(TokenType::LBRACE)) throw std::runtime_error("Expected '{'");

    std::vector<StmtPtr> body;
    while (current().type != TokenType::RBRACE && current().type != TokenType::END) {
        body.push_back(parseStatement());
    }
    if (!match(TokenType::RBRACE)) throw std::runtime_error("Expected '}'");

    return std::make_unique<IfStmt>(std::move(condition), std::move(body));
}

StmtPtr Parser::parseWhileStatement() {
    if (!match(TokenType::KW_WHILE)) throw std::runtime_error("Expected 'while'");
    if (!match(TokenType::LPAREN)) throw std::runtime_error("Expected '('");

    auto condition = parseExpression();

    if (!match(TokenType::RPAREN)) throw std::runtime_error("Expected ')'");
    if (!match(TokenType::LBRACE)) throw std::runtime_error("Expected '{'");

    std::vector<StmtPtr> body;
    while (current().type != TokenType::RBRACE && current().type != TokenType::END) {
        body.push_back(parseStatement());
    }

    if (!match(TokenType::RBRACE)) throw std::runtime_error("Expected '}'");

    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

DeclPtr Parser::parseFunction() {
    if (!match(TokenType::KW_INT)) throw std::runtime_error("Expected return type");
    if (!match(TokenType::IDENTIFIER)) throw std::runtime_error("Expected function name");

    std::string name = previous().lexeme;

    if (!match(TokenType::LPAREN)) throw std::runtime_error("Expected '('");

    std::vector<std::string> parameters;
    if (current().type != TokenType::RPAREN) {
        while (true) {
            if (!match(TokenType::KW_INT)) throw std::runtime_error("Expected parameter type");
            if (!match(TokenType::IDENTIFIER)) throw std::runtime_error("Expected parameter name");
            parameters.push_back(previous().lexeme);

            if (!match(TokenType::COMMA)) break;
        }
    }

    if (!match(TokenType::RPAREN)) throw std::runtime_error("Expected ')'");
    if (!match(TokenType::LBRACE)) throw std::runtime_error("Expected '{'");

    std::vector<StmtPtr> body;
    while (current().type != TokenType::RBRACE && current().type != TokenType::END)
        body.push_back(parseStatement());

    if (!match(TokenType::RBRACE)) throw std::runtime_error("Expected ')'");

    return std::make_unique<FunctionDecl>(name, std::move(parameters), std::move(body));
}
