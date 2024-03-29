#pragma once

#include "../parser/grammar.hpp"
#include "SFML/Graphics.hpp"
#include <regex>

class TerminalNode;
class NonTerminalNode;
class FlatListNode;

extern sf::Font font;

const int FONT_SIZE = 20;

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& v);

/////////////// RENDER CHUNKS

class RenderChunk {
    public:
    sf::Vector2f position; // relative to the parent

    RenderChunk(sf::Vector2f pos);
    virtual sf::Vector2f ComputeSize() = 0;
    virtual void Render(sf::RenderWindow& window, sf::Vector2f offset) = 0;
};

struct RenderChunkRectangle 
    : public RenderChunk
{
    sf::RectangleShape rect;

    RenderChunkRectangle(sf::RectangleShape rect, sf::Color c, sf::Vector2f offset);
    sf::Vector2f ComputeSize();
    void Render(sf::RenderWindow& window, sf::Vector2f offset) override;
};

struct RenderChunkText
    : public RenderChunk
{
    sf::Text text;

    RenderChunkText(std::string text, sf::Vector2f offset);
    sf::Vector2f ComputeSize();
    void Render(sf::RenderWindow& window, sf::Vector2f offset) override;
};

struct RenderChunkList
    : public RenderChunk
{
    std::vector<std::unique_ptr<RenderChunk>> children;

    RenderChunkList(sf::Vector2f pos);
    sf::Vector2f ComputeSize();
    void Render(sf::RenderWindow& window, sf::Vector2f offset) override;
};

/////////////// DISPLAY NODES

class DisplayNode {
public:
    virtual std::string ToString() = 0;
    virtual std::unique_ptr<RenderChunk> Render() = 0;
};

class TerminalNode 
    : public DisplayNode
{
public:
    std::string ToString() override;
    std::unique_ptr<RenderChunk> Render() override;

    Terminal* terminal = nullptr;
    std::string value;
};

class NonTerminalNode
    : public DisplayNode
{
public:
    std::string ToString() override;
    std::unique_ptr<RenderChunk> Render() override;

    Production myProduction();

    NonTerminal* nonTerminal  = nullptr;
    std::vector<std::unique_ptr<DisplayNode>> children;
    int prodNumber = -1;
};

class FlatListNode
    : public DisplayNode
{
public:
    std::string ToString() override;
    std::unique_ptr<RenderChunk> Render() override;

    FlatList* flatList = nullptr;
    std::vector<std::unique_ptr<DisplayNode>> children;
};
