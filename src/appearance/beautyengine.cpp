#include "beautyengine.hpp"
#include "../structures/structures.hpp"
#include <iostream>

static sf::Font font;
static NonTerminalNode root;

void BeautyEngine::Init(GrammarDescription* gd) {
    if (!font.loadFromFile("../data/fonts/Inconsolata-Regular.ttf"))
    {
        std::cout << "failed to load font\n";
    }
    
    this->grammarDesc = gd;

    std::cout << this->grammarDesc->GetNuts().size() << "\n";
}

void BeautyEngine::Repaint(sf::RenderWindow& window) {
    sf::Text text;
    text.setFont(font); // font is a sf::Font
    text.setString("Hello world");
    text.setCharacterSize(24); // in pixels, not points!
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    window.draw(text);
}