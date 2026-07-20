#pragma once

#include "basic_block.h"

#include <vector>

struct CFG {
    std::vector<BasicBlock> blocks;
};
