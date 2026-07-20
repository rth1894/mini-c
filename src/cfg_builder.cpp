#include "../include/cfg_builder.h"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>

CFG CFGBuilder::build(const TACProgram& program) {
    std::unordered_map<std::string, int> labelToBlock;
    CFG cfg;
    if (program.empty()) return cfg;

    std::unordered_set<std::size_t> leaders;
    leaders.insert(0);

    for (std::size_t i=0; i<program.size(); i++) {
        const auto& inst = program[i];
        if (inst.op == "label") leaders.insert(i);

        if ((inst.op == "goto" || inst.op == "ifFalse" || inst.op == "return") && i + 1 < program.size())
            leaders.insert(i + 1);
    }

    BasicBlock current;
    int blockNumber = 0;
    for (size_t i=0; i<program.size(); i++) {
        if (leaders.count(i)) {
            if (!current.instructions.empty()) {
                cfg.blocks.push_back(std::move(current));
                current = BasicBlock();
            }
            current.name = "B" + std::to_string(blockNumber++);
        }
        current.instructions.push_back(program[i]);
    }
    if (!current.instructions.empty()) cfg.blocks.push_back(std::move(current));

    for (int i=0; i<static_cast<int>(cfg.blocks.size()); i++) {
        for (const auto& inst : cfg.blocks[i].instructions)
            if (inst.op == "label") labelToBlock[inst.result] = i;
    }

    for (int i=0; i<static_cast<int>(cfg.blocks.size()); i++) {
        auto& block = cfg.blocks[i];
        if (block.instructions.empty()) continue;
        const auto& last = block.instructions.back();

        // unconditional jmp
        if (last.op == "goto") {
            auto it = labelToBlock.find(last.result);
            if (it == labelToBlock.end()) throw std::runtime_error("Unknown label '" + last.result + "'");
            int target = it->second;
            block.successors.push_back(target);
            cfg.blocks[target].predecessors.push_back(i);
        }

        // conditional jmp
        else if (last.op == "ifFalse") {
            auto it = labelToBlock.find(last.result);
            if (it == labelToBlock.end()) throw std::runtime_error("Unknown label '" + last.result + "'");
            int target = it->second;
            block.successors.push_back(target);
            cfg.blocks[target].predecessors.push_back(i);

            if (i + 1 < static_cast<int>(cfg.blocks.size())) {
                block.successors.push_back(i + 1);
                cfg.blocks[i + 1].predecessors.push_back(i);
            }
        }

        // return has no sucessors
        else if (last.op == "return") continue;

        // fall through
        else if (i + 1 < static_cast<int>(cfg.blocks.size())) {
            block.successors.push_back(i + 1);
            cfg.blocks[i+1].predecessors.push_back(i);
        }
    }

    return cfg;
}
