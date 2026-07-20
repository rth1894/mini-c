#include "../include/cfg_printer.h"

#include <iostream>

void CFGPrinter::print(const CFG& cfg) {
    for (const auto& block : cfg.blocks) {
        std::cout << block.name << " (" << block.instructions.size() << " instructions)\n";
        std::cout << "-----\n";
        for (const auto& inst : block.instructions) {
            if (inst.op == "label") std::cout << inst.result << ":\n";
            else if (inst.op == "goto") std::cout << "goto " << inst.result << "\n";
            else if (inst.op == "ifFalse") std::cout << "ifFalse " << inst.arg1 << " goto " << inst.result << "\n";
            else if (inst.op == "return") std::cout << "return " << inst.arg1 << "\n";
            else if (inst.op == "call") std::cout << inst.result << " = call " << inst.arg1 << ", " << inst.arg2 << "\n";
            else if (inst.op == "function") std::cout << "function " << inst.result << "\n";
            else if (inst.op == "=") std::cout << inst.result << " = " << inst.arg1 << "\n";

            else std::cout << inst.result << " = " << inst.arg1 << " " << inst.op << " " << inst.arg2 << "\n";
        }
        std::cout << "\nSuccessors: ";
        for (int suc : block.successors) std::cout << cfg.blocks[suc].name << " ";

        std::cout << "\nPredecessors: ";
        for (int pred : block.predecessors) std::cout << cfg.blocks[pred].name << " ";
        std::cout << "\n-----";
    }
    std::cout << "\n";

}

/*
INPUT TAC      |                   B0                          B2
                                   -----                       ------
t0 = 5         |                  t0 = 5                       L1:
t1 = 8         |                  ifFalse t0 goto L1           t2 = 10
goto L2        |   -- OUTPUT -->
label L1       |                  B1                           B3
t2 = 10        |                  -----                        -----
label L2       |                  t1 = 8                       L2:
return t2      |                  goto L2                      return t2

*/
