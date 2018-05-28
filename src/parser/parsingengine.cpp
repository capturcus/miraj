#include <iostream> // debugging

#include "parsingengine.hpp"

void ParsingEngine::consumeToken(Token* token) {
    std::cout << "CONSUMED " << token->type << " " << token->value << "\n";
}
