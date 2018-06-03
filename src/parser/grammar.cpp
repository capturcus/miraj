#include "grammar.hpp"
#include <iostream>

void Terminal::dump() const {
    std::cout << "TERM " << lhs << "\n";
}

void NonTerminal::dump() const {
    std::cout << "NONTERM " << lhs << " [\n";
    for (auto& prod : productions) {
        std::cout << prod.number << ": ";
        for (auto& n : prod.nuts) {
            std::cout << n->lhs << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "]\n";
}