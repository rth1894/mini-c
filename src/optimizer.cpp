#include "../include/optimizer.h"

#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

bool Optimizer::isNumber(const std::string& value) {
    if (value.empty()) return false;

    for (char c : value)
        if (!std::isdigit(c)) return false;
    return true;
}

int Optimizer::evaluate(const std::string& op, int lhs, int rhs) {
    if (op == "+") return lhs + rhs;
    if (op == "-") return lhs - rhs;
    if (op == "*") return lhs * rhs;
    if (op == "/") return lhs / rhs;
    throw std::runtime_error("Unknown operator");
}

// constant folding
TACProgram Optimizer::optimize(const TACProgram& program) {
    TACProgram optimized, result;
    std::unordered_map<std::string, std::string> constants;
    std::unordered_set<std::string> live;

    for (auto inst : program) {
        if (constants.count(inst.arg1)) inst.arg1 = constants[inst.arg1];
        if (constants.count(inst.arg2)) inst.arg2 = constants[inst.arg2];
        if (isNumber(inst.arg1) && isNumber(inst.arg2) &&
                (inst.op == "+" || inst.op == "-" || inst.op == "*" || inst.op == "/")) {
            std::string folded = std::to_string(evaluate(inst.op, std::stoi(inst.arg1), std::stoi(inst.arg2)));
            constants[inst.result] = folded;
            optimized.push_back({"=", inst.result, folded, ""});
            continue;
        }

        if (inst.op == "=" && isNumber(inst.arg1)) constants[inst.result] = inst.arg1;
        else if (!inst.result.empty()) constants.erase(inst.result);
        optimized.push_back(inst);
    }

    for (auto it = optimized.rbegin(); it != optimized.rend(); it++) {
        const auto& inst = *it;
        if (inst.op == "return") {
            if (!inst.arg1.empty() && !isNumber(inst.arg1)) live.insert(inst.arg1);
            result.push_back(inst);
            continue;
        }
        if (inst.op == "call") {
            if (!inst.arg1.empty()) live.insert(inst.arg1);
            result.push_back(inst);
            continue;
        }
        if (inst.op == "label" || inst.op == "goto" || inst.op == "ifFalse" || inst.op == "function") {
            if (inst.op == "ifFalse" && !isNumber(inst.arg1)) live.insert(inst.arg1);
            result.push_back(inst);
            continue;
        }
        if (!inst.result.empty()) {
            if (live.find(inst.result) == live.end()) continue;
            live.erase(inst.result);
        }
        if (!inst.arg1.empty() && !isNumber(inst.arg1)) live.insert(inst.arg1);
        if (!inst.arg2.empty() && !isNumber(inst.arg2)) live.insert(inst.arg2);
        result.push_back(inst);

    }
    std::reverse(result.begin(), result.end());
    return result;
}
