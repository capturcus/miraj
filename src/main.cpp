#include <cstdio>
#include <cstdlib>
#include <locale>
#include <memory>
#include <iostream>
#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "text/coloredchar.hpp"
#include "lexer/lexingengine.hpp"
#include "parser/parsingengine.hpp"

#include "structures/linelist.hpp"

template<typename T>
static inline std::vector<T> operator+(const std::vector<T> & a, const std::vector<T> & b)
{
    std::vector<T> ret;
    ret.reserve(a.size() + b.size());
    ret = a;
    ret.insert(ret.end(), b.begin(), b.end());
    return ret;
}

LineList<std::vector<coloredChar32_t >> list;
std::map<uint8_t, std::pair<sf::Color, sf::Color>> palette {
        { 0, { sf::Color::White, sf::Color::Transparent } },
        { 1, { sf::Color::Black, sf::Color::Red } },
        { 2, { sf::Color::Red, sf::Color::Transparent } },
};
sf::Font font;

unsigned int numberLength(unsigned int i)
{
    unsigned int ret = 0;
    while (i != 0) {
        i /= 10;
        ++ret;
    }

    return std::max(1U, ret);
}

void repaint(sf::RenderTarget & rt)
{
    const unsigned int CHARACTER_SIZE = 16;
    const float LINE_SPACING = font.getLineSpacing(CHARACTER_SIZE);

    rt.clear();

    sf::RectangleShape rect;
    rect.setOrigin(0.f, 0.f);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(CHARACTER_SIZE);
    text.setFillColor(sf::Color::White);

    float yOffset = 0.f;

    const auto lineCount = list.size();
    std::string line;
    for (int64_t l = 0; l < lineCount; ++l) {
        uint8_t prevColor = 255;
        float xOffset = 0.f;

        line.clear();

        auto paintFragment = [&]() {
            if (line.empty()) {
                return;
            }

            const auto fillColor = palette[prevColor].first;
            const auto bgColor = palette[prevColor].second;

            // Setup text position and string to help rect calculate its size
            text.setString(line);
            text.setPosition(xOffset, yOffset);

            // Draw rectangle
            rect.setPosition(xOffset, yOffset);
            rect.setSize({ text.findCharacterPos(line.size()).x - xOffset, LINE_SPACING });
            rect.setFillColor(bgColor);
            rt.draw(rect);

            // Draw the text
            text.setFillColor(fillColor);
            rt.draw(text);

            // Advance xOffset
            xOffset = text.findCharacterPos(line.size()).x;
            line.clear();
        };

        for (size_t col = 0; col < list[l].size(); ++col) {
            const auto & c = list[l][col];
            if (c.color != prevColor) {
                paintFragment();
                prevColor = c.color;
            }
            line.push_back((char)list[l][col].code);
        }

        paintFragment();
        yOffset += LINE_SPACING;
    }
}

template<typename T>
static std::vector<coloredChar_t<T>> colorize(uint8_t color, const T * sz)
{
    std::vector<coloredChar_t<T>> ret;
    ret.reserve(std::char_traits<T>::length(sz));

    while (*sz != T()) {
        ret.push_back(coloredChar_t<T>{ *sz, color });
        ++sz;
    }

    return ret;
}

int main(int argc, char ** argv)
{
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "lol");
    // Font
    font.loadFromFile("../data/fonts/Inconsolata-Regular.ttf");

    list.append(colorize(0, U"int main(int argc, char ** argv) {"));
    list.append(
            colorize(0, U"    ") +
            colorize(2, U"return ") +
            colorize(1, U"0") +
            colorize(0, U";")
    );
    list.append(colorize(0, U"}"));

    ParsingEngine parsingEngine;
    try {
        parsingEngine.InitGrammar("../data/grammar.json");
    }
    catch (const char* c) {
        printf("Caught string exception: %s\n", c);
        return 1;
    }

    window.setKeyRepeatEnabled(false);

    while (window.isOpen()) {
        sf::Event event;
        if (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::TextEntered) {
                parsingEngine.ProcessKeypress(event.text.unicode);
            }
            repaint(window);
            window.display();
        }
    }

    return 0;
}

