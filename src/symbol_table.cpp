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
