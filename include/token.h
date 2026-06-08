#pragma once

#include <string>

enum class TokenType {
    END,

    IDENTIFIER,
    INTEGER,

    KW_INT,
    KW_RETURN,
    KW_IF,
    KW_ELSE,
    KW_WHILE,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    ASSIGN,

    EQ,
    NEQ,
    LT,
    GT,
    LE,
    GE,

    LPAREN,
    RPAREN,

    LBRACE,
    RBRACE,

    COMMA,
    SEMICOLON
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};
