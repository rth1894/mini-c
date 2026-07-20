#pragma once

#include "tac.h"

#include <string>
#include <vector>

struct BasicBlock {
    std::string name;
    std::vector<TACInstruction> instructions;

    std::vector<int> predecessors;
    std::vector<int> successors;
};
