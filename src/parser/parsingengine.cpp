#include <iostream> // debugging

#include "parsingengine.hpp"

void ParsingEngine::ProcessKeypress(char32_t c) {
    auto tokens = lexingEngine->ProcessKeypress(c);
    for (auto& token : tokens) {
        std::cout << "CONSUMED " << token << "\n";
    }
}

ParsingEngine::ParsingEngine() {
    lexingEngine = std::make_unique<LexingEngine>();
}