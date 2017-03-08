#pragma once

#include <utility>
#include <string>

class LexingEngine;

class ParsingEngine {
public:
    void consumeToken(std::pair<std::string, std::string>);

    LexingEngine* lexingEngine = nullptr;
};
