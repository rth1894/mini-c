#include <fstream>
#include <iostream>

// #include "../include/ast_printer.h"
#include "../include/ir_generator.h"
#include "../include/ir_printer.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/semantic_analyzer.h"
#include "../include/type_checker.h"

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

    try {
        Parser parser(tokens);
        auto program = parser.parseProgram();

        SemanticAnalyzer semantic;
        semantic.analyze(program.get());

        TypeChecker checker;
        checker.check(program.get());

        IRGenerator generator;
        auto ir = generator.generate(program.get());
        IRPrinter printer;
        printer.print(ir);
        /*
         *
        ASTPrinter printer;
        printer.print(program.get());
        */
    }
    catch (const std::exception& e) {
        std::cerr << "Error:\n" << e.what() << "\n";
        return 1;
    }
}
