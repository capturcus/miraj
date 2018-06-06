#pragma once

#include <SFML/Graphics.hpp>
#include "../parser/grammar.hpp"

class BeautyEngine {
public:
    void Repaint(sf::RenderWindow& window);

    void Init(GrammarDescription*);

    GrammarDescription* grammarDesc;
};