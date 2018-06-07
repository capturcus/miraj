#include "beautyengine.hpp"
#include <iostream>

static sf::Font font;
static NonTerminalNode root;

TerminalNode BeautyEngine::debugMakeFixedTerminalNode(std::string name) {
    TerminalNode ret;
    ret.terminal = this->grammarDesc->GetNuts().at(name)->AsTerminal();
    ret.value = ret.terminal->GetValue();
    return ret;
}


void BeautyEngine::Init(GrammarDescription* gd) {
    if (!font.loadFromFile("../data/fonts/Inconsolata-Regular.ttf"))
    {
        std::cout << "failed to load font\n";
    }
    
    this->grammarDesc = gd;

    auto flatList = new FlatListNode();
    flatList->flatList = this->grammarDesc->GetNuts().at("components")->AsFlatList();
    {
        auto t = debugMakeFixedTerminalNode("IDENTIFIER");
        t.value = "asd";
        flatList->children.push_back(std::make_unique<TerminalNode>(t));
    }
    {
        auto t = debugMakeFixedTerminalNode("IDENTIFIER");
        t.value = "dsa";
        flatList->children.push_back(std::make_unique<TerminalNode>(t));
    }
    {
        auto t = debugMakeFixedTerminalNode("NUMERAL");
        t.value = "123";
        flatList->children.push_back(std::make_unique<TerminalNode>(t));
    }

    auto flatList2 = new FlatListNode();

    auto ifStmt = NonTerminalNode();
    ifStmt.nonTerminal = this->grammarDesc->GetNuts().at("if_stmt").get()->AsNonTerminal();
    ifStmt.children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("IF")));
    ifStmt.children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("LPAREN")));
    ifStmt.children.push_back(std::unique_ptr<FlatListNode>(flatList));
    ifStmt.children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("RPAREN")));
    ifStmt.children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("LBRACE")));
    // ifStmt.children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("RBRACE")));

    root.nonTerminal = this->grammarDesc->GetNuts().at("start").get()->AsNonTerminal();

    std::cout << "ROOT TOSTRING " << root.ToString() << "\n";
}

void BeautyEngine::Repaint(sf::RenderWindow& window) {
    sf::Text text;
    text.setFont(font); // font is a sf::Font
    text.setString("Hello world");
    text.setCharacterSize(24); // in pixels, not points!
    text.setFillColor(sf::Color::Red);
    // text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    window.draw(text);
}