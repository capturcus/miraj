#pragma once

#include <utility>
#include <string>

#include "../lexer/lexingengine.hpp"

class LexingEngine;

class ParsingEngine {
public:
    ParsingEngine();
    void ConsumeToken(Token t);
    void ProcessKeypress(char32_t c);

    std::unique_ptr<LexingEngine> lexingEngine;
};
