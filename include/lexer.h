#pragma once

#include <string>
#include <vector>

#include "token.h"

class Lexer {
public:
    explicit Lexer(const std::string& source);

    std::vector<Token> tokenize();

private:
    char current() const;
    char peek() const;

    void advance();
    void skipWhitespace();

    Token identifier();
    Token number();

    std::string source_;

    size_t pos_ = 0;

    int line_ = 1;
    int col_ = 1;
};
