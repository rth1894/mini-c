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

        bool declareFunction(const std::string& name, std::size_t parameterCount);
        bool functionExists(const std::string& name) const;
        std::size_t parameterCount(const std::string& name) const;

    private:
        std::vector<std::unordered_map<std::string, bool>> scopes_;
        std::unordered_map<std::string, std::size_t> functions_;
};
