#pragma once

#include "tac.h"

class Optimizer {
    public:
        TACProgram optimize(const TACProgram& program);
    private:
        TACProgram consantFold(const TACProgram& program);
        TACProgram consantPropogate(const TACProgram& program);
        TACProgram consantEliminate(const TACProgram& program);

        bool isNumber(const std::string& value);
        int evaluate(const std::string& op, int lhs, int rhs);
};
