#pragma once

#include <utility>
#include <string>

#include "../lexer/lexingengine.hpp"
#include "grammar.hpp"

class LexingEngine;

class ParsingEngine {
public:
    ParsingEngine();
    void ConsumeToken(Token t);
    void ProcessKeypress(char32_t c);
    void InitGrammar(std::string path);

private:
    Nut* makeAndGetNut(std::string lhs, bool terminal);
    Nut* getNut(std::string lhs);
    std::unique_ptr<LexingEngine> lexingEngine;
    std::vector<std::unique_ptr<Nut>> nuts;
};
