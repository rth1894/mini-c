#include "../include/symbol_table.h"

bool SymbolTable::declare(const std::string& name) {
    return symbols_.insert({name, true}).second;
}

bool SymbolTable::exist(const std::string& name) const {
    return symbols_.find(name) != symbols_.end();
}
