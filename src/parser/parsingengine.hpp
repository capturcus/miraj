#pragma once

#include <map>
#include <string>
#include <utility>

#include "../lexer/lexingengine.hpp"
#include "grammar.hpp"

class LexingEngine;

class ParsingEngine {
public:
    ParsingEngine();
    void ProcessKeypress(char32_t c);
    void InitGrammar(const std::string& path);
    
    GrammarDescription gdesc;

private:
    std::unique_ptr<LexingEngine> lexingEngine;
};
