#include "../include/lexer.h"

#include <cctype>
#include <unordered_map>

Lexer::Lexer(const std::string& source)
    : source_(source) {}

char Lexer::current() const {
    if (pos_ >= source_.size()) return '\0';
    return source_[pos_];
}

char Lexer::peek() const {
    if (pos_ + 1 >= source_.size()) return '\0';
    return source_[pos_ + 1];
}

void Lexer::advance() {
    if (current() == '\n') {
        line_++;
        col_ = 1;
    }
    else col_++;
    pos_++;
}

void Lexer::skipWhitespace() {
    while (std::isspace(current()))
        advance();
}

Token Lexer::number() {
    int start = col_;
    std::string value;
    while (std::isdigit(current())) {
        value += current();
        advance();
    }

    return {
        TokenType::INTEGER,
        value,
        line_,
        start
    };
}

Token Lexer::identifier() {
    int start = col_;
    std::string value;

    while (std::isalnum(current()) || current() == '_') {
        value += current();
        advance();
    }

    static const std::unordered_map< std::string, TokenType > keywords = {
        {"int", TokenType::KW_INT},
        {"return", TokenType::KW_RETURN},
        {"if", TokenType::KW_IF},
        {"else", TokenType::KW_ELSE},
        {"while", TokenType::KW_WHILE}
    };
    auto it = keywords.find(value);

    if (it != keywords.end()) {
        return {
            it->second,
            value,
            line_,
            start
        };
    }

    return {
        TokenType::IDENTIFIER,
        value,
        line_,
        start
    };
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (current()) {
        skipWhitespace();

        if (!current()) break;

        if (std::isdigit(current())) {
            tokens.push_back(number());
            continue;
        }

        if (std::isalpha(current()) || current() == '_') {
            tokens.push_back(identifier());
            continue;
        }

        switch (current()) {
            case '+':
                tokens.push_back({TokenType::PLUS, "+", line_, col_});
                advance();
                break;

            case '-':
                tokens.push_back({TokenType::MINUS, "-", line_, col_});
                advance();
                break;

            case '*':
                tokens.push_back({TokenType::STAR, "*", line_, col_});
                advance();
                break;

            case '/':
                tokens.push_back({TokenType::SLASH, "/", line_, col_});
                advance();
                break;

            case '=':
                tokens.push_back({TokenType::ASSIGN, "=", line_, col_});
                advance();
                break;

            case '(':
                tokens.push_back({TokenType::LPAREN, "(", line_, col_});
                advance();
                break;

            case ')':
                tokens.push_back({TokenType::RPAREN, ")", line_, col_});
                advance();
                break;

            case '{':
                tokens.push_back({TokenType::LBRACE, "{", line_, col_});
                advance();
                break;

            case '}':
                tokens.push_back({TokenType::RBRACE, "}", line_, col_});
                advance();
                break;

            case ';':
                tokens.push_back({TokenType::SEMICOLON, ";", line_, col_});
                advance();
                break;

            default:
                advance();
        }
    }

    tokens.push_back({TokenType::END, "", line_, col_});
    return tokens;
}
