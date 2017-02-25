#include <cstdio>
#include <cstdlib>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "text/document.hpp"
#include "text/vvdocument.hpp"

std::shared_ptr<Document> doc;
sf::Font font;

unsigned int numberLength(unsigned int i)
{
    unsigned int ret = 0;
    while (i != 0) {
        i /= 10;
        ++ret;
    }

    return std::max(1, ret);
}

void repaint(sf::RenderTarget & rt)
{
    rt.clear();

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);

    float linePos = 0.f;

    const auto lineCount = doc->lineCount();
    std::string line;
    for (int64_t l = 0; l < lineCount; ++l) {
        line.clear();

        auto pos = doc->fromLineAndColumn(l, 0);
        char32_t c = doc->at(pos);

        while (c != Document::eol) {
            line += (char)c;
            ++pos;
            c = doc->at(pos);
        }

        text.setString(line);
        text.setPosition(0, linePos);
        rt.draw(text);

        linePos += 16;
    }
}

int main(int argc, char ** argv)
{
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "lol");
    // Font
    font.loadFromFile("../data/fonts/Inconsolata-Regular.ttf");

    doc = std::make_shared<VVDocument>("int main(int argc, char ** argv) {\n    return 0;\n}");

    while (window.isOpen()) {
        sf::Event event;
        if (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            repaint(window);
            window.display();
        }
    }

    return 0;
}

