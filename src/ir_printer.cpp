#include "../include/ir_printer.h"

#include <iostream>

void IRPrinter::print(const TACProgram& program) {
    for (const auto& inst : program) {
        if (inst.op == "return") {
            std::cout << "return " << inst.arg1 << "\n";
            continue;
        }
        if (inst.op == "=") {
            std::cout << inst.result << " = " << inst.arg1 << "\n";
            continue;
        }
        if (inst.op == "param") {
            std::cout << "param " << inst.arg1 << "\n";
            continue;
        }
        if (inst.op == "call") {
            std::cout << inst.result << " = call " << inst.arg1 << ", " << inst.arg2 << "\n";
            continue;
        }
        if (inst.op == "function") {
            std::cout << "\nfunction " << inst.result << ":\n";
            continue;
        }
        if (inst.op == "label") {
            std::cout << inst.result << ":\n";
            continue;
        }
        if (inst.op == "goto") {
            std::cout << "goto " << inst.result << "\n";
            continue;
        }
        if (inst.op == "ifFalse") {
            std::cout << "ifFalse " << inst.arg1 << " goto " << inst.result << "\n";
            continue;
        }

        else std::cout << inst.result << " = " << inst.arg1 << " " << inst.op << " " << inst.arg2 << "\n";
    }
}
