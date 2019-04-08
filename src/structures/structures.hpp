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
    sf::Text text;
    sf::Color rectColor = sf::Color(0,0,0);
    std::vector<std::unique_ptr<RenderChunk>> children;
    sf::Vector2f position; // relative to the parent

    sf::Vector2f ComputeSize();
    void AddOffset(sf::Vector2f offset);
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
    virtual std::unique_ptr<RenderChunk> Render() = 0;
};

class TerminalNode 
    : public DisplayNode
{
public:
    virtual TerminalNode* AsTerminalNode() override {
        return this;
    }

    std::string ToString() override;
    std::unique_ptr<RenderChunk> Render() override;

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
    std::unique_ptr<RenderChunk> Render() override;

    NonTerminal* nonTerminal  = nullptr;
    std::vector<std::unique_ptr<DisplayNode>> children;
    int prodNumber = -1;
};

class FlatListNode
    : public DisplayNode
{
public:
    virtual FlatListNode* AsFlatListNode() override  {
        return this;
    }

    std::string ToString() override;
    std::unique_ptr<RenderChunk> Render() override;

    FlatList* flatList = nullptr;
    std::vector<std::unique_ptr<DisplayNode>> children;
};

std::ostream& operator<< (std::ostream &out, const sf::Vector2f& v);
std::ostream& operator<< (std::ostream &out, const sf::FloatRect& r);