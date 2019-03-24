#pragma once

#include <SFML/Graphics.hpp>
#include "../parser/grammar.hpp"
#include "../structures/structures.hpp"

class BeautyEngine {
public:
    void Repaint(sf::RenderWindow& window);

    void Init(GrammarDescription*);

    GrammarDescription* grammarDesc;

private:
    TerminalNode debugMakeFixedTerminalNode(std::string name);
};
