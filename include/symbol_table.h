#pragma once

#include <string>
#include <unordered_map>

class SymbolTable {
    public:
        bool declare(const std::string& name);
        bool exist(const std::string& name) const;

    private:
        std::unordered_map<std::string, bool> symbols_;
};
