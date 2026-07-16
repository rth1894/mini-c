#include "../include/symbol_table.h"

SymbolTable::SymbolTable() {
    pushScope();
}

void SymbolTable::pushScope() {
    scopes_.emplace_back();
}

void SymbolTable::popScope() {
    if (!scopes_.empty()) scopes_.pop_back();
}

bool SymbolTable::declare(const std::string& name) {
    auto& currentScope = scopes_.back();
    return currentScope.insert({name, true}).second;
}

bool SymbolTable::exist(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); it++) {
        if (it->find(name) != it->end()) return true;
    }
    return false;
}

bool SymbolTable::functionExists(const std::string& name) const {
    return functions_.find(name) != functions_.end();
}

bool SymbolTable::declareFunction(const std::string& name, std::size_t parameterCount) {
    return functions_.insert({name, parameterCount}).second;
}

std::size_t SymbolTable::parameterCount (const std::string& name) const {
    return functions_.at(name);
}
