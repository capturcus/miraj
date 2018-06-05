#include <cstdio>
#include <cstdlib>
#include <locale>
#include <memory>
#include <iostream>
#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "lexer/lexingengine.hpp"
#include "parser/parsingengine.hpp"

template<typename T>
static inline std::vector<T> operator+(const std::vector<T> & a, const std::vector<T> & b)
{
    std::vector<T> ret;
    ret.reserve(a.size() + b.size());
    ret = a;
    ret.insert(ret.end(), b.begin(), b.end());
    return ret;
}

int main(int argc, char ** argv)
{
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "lol");

    ParsingEngine parsingEngine;
    parsingEngine.InitGrammar("../data/grammar.json");

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
            window.display();
        }
    }

    return 0;
}

