#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class SymbolTable {
    public:
        SymbolTable();
        void pushScope();
        void popScope();

        bool declare(const std::string& name);
        bool exist(const std::string& name) const;

    private:
        std::vector<std::unordered_map<std::string, bool>> scopes_;
};
