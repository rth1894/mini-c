#include <fstream>
#include <iostream>

#include "../include/ast_printer.h"
#include "../include/cfg_builder.h"
#include "../include/cfg_printer.h"
#include "../include/ir_generator.h"
#include "../include/ir_printer.h"
#include "../include/lexer.h"
#include "../include/llvm_generator.h"
#include "../include/optimizer.h"
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

        Optimizer optimizer;
        auto optimizedIR = optimizer.optimize(ir);

        CFGBuilder builder;
        CFG cfg = builder.build(optimizedIR);
        CFGPrinter cfgPrinter;
        cfgPrinter.print(cfg);

        LLVMGenerator llvmGen;
        std::string llvmIR = llvmGen.generate(program.get());
        std::ofstream out("output.ll");
        out << llvmIR;
        out.close();

        std::cout << "Generated LLVM IR: output.ll";

        IRPrinter irPrinter;
        std::cout << "=== RAW IR ===";
        irPrinter.print(ir);

        std::cout << "=== OPTIMIZED IR ===";
        irPrinter.print(optimizedIR);

        ASTPrinter printer;
        printer.print(program.get());
    }
    catch (const std::exception& e) {
        std::cerr << "Error:\n" << e.what() << "\n";
        return 1;
    }
}
