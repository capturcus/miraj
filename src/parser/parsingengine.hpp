#pragma once

#include <utility>
#include <string>

#include "../lexer/lexingengine.hpp"

class LexingEngine;

class ParsingEngine {
public:
    void consumeToken(Token* t);

    LexingEngine* lexingEngine = nullptr;
};
