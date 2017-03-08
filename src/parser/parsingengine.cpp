#include <iostream> // debugging

#include "parsingengine.hpp"

void ParsingEngine::consumeToken(std::pair<std::string, std::string> token) {
    std::cout << "CONSUMED " << token.first << " " << token.second << "\n";
}
