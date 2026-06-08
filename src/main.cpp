#include <fstream>
#include <iostream>

#include "../include/ast_printer.h"
#include "../include/lexer.h"
#include "../include/parser.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: minicpp file.mc\n";
        return 1;
    }

    std::ifstream in(argv[1]);

    std::string source( (std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>()
    );

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto stmt = parser.parseStatement();

    ASTPrinter printer;
    printer.print(stmt.get());
}
