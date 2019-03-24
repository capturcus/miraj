#pragma once

#include "../parser/grammar.hpp"
#include "SFML/Graphics.hpp"

class TerminalNode;
class NonTerminalNode;
class FlatListNode;

extern sf::Font font;

struct ColoredRectangle {
    sf::RectangleShape rect;
    float opacity;
    sf::Color color;
    void Render(sf::RenderWindow& window);
};

class RenderChunk {
    public:
    std::vector<sf::Text> texts;
    std::vector<ColoredRectangle> rects;
    std::vector<std::unique_ptr<RenderChunk>> children;
    sf::Vector2f position; // within parent

    sf::Vector2f ComputeSize();
    void Render(sf::RenderWindow& window);
};

class DisplayNode {
public:
    virtual TerminalNode* AsTerminalNode() {
        return nullptr;
    }

    virtual NonTerminalNode* AsNonTerminalNode() {
        return nullptr;
    }

    virtual FlatListNode* AsFlatListNode() {
        return nullptr;
    }

    virtual std::string ToString() = 0;
    virtual std::unique_ptr<RenderChunk> Render(sf::Vector2f offset) = 0;
};

class TerminalNode 
    : public DisplayNode
{
public:
    virtual TerminalNode* AsTerminalNode() override {
        return this;
    }

    std::string ToString() override;
    std::unique_ptr<RenderChunk> Render(sf::Vector2f offset) override;

    Terminal* terminal = nullptr;
    std::string value;
};

class NonTerminalNode
    : public DisplayNode
{
public:
    virtual NonTerminalNode* AsNonTerminalNode() override {
        return this;
    }

    std::string ToString() override;
    std::unique_ptr<RenderChunk> Render(sf::Vector2f offset) override;

    NonTerminal* nonTerminal  = nullptr;
    std::vector<std::unique_ptr<DisplayNode>> children;
    int prodNumber;
};

class FlatListNode
    : public DisplayNode
{
public:
    virtual FlatListNode* AsFlatListNode() override  {
        return this;
    }

    std::string ToString() override;
    std::unique_ptr<RenderChunk> Render(sf::Vector2f offset) override;

    FlatList* flatList = nullptr;
    std::vector<std::unique_ptr<DisplayNode>> children;
};