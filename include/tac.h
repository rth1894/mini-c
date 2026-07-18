/*
 * return a + b * 3;
 *
 * becomes:
 * t0 = b * 3
 * t1 = a + t0
 * return t1
 *
 * so that each instruction has atmost one op
*/
#pragma once

#include <string>
#include <vector>

struct TACInstruction {
    std::string op;
    std:: string result;
    std::string arg1;
    std::string arg2;

    /*
     for t0 = b * 3 -> op="*",      result="t0", arg1="b",   arg2="3"
     for x = t0     -> op="=",      result="x",  arg1="t0",  arg2=""
     for return t1  -> op="return", result="",   arg1="t1",  arg2=""
    */
};

using TACProgram = std::vector<TACInstruction>;
