#include "beautyengine.hpp"
#include <iostream>

sf::Font font;
static NonTerminalNode root;

TerminalNode BeautyEngine::debugMakeFixedTerminalNode(std::string name)
{
    TerminalNode ret;
    ret.terminal = this->grammarDesc->GetNuts().at(name)->AsTerminal();
    ret.value = ret.terminal->GetValue();
    return ret;
}

void BeautyEngine::Init(GrammarDescription *gd)
{
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

    auto ifStmt = new NonTerminalNode();
    ifStmt->nonTerminal = this->grammarDesc->GetNuts().at("if_stmt").get()->AsNonTerminal();
    ifStmt->children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("IF")));
    ifStmt->children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("LPAREN")));
    ifStmt->children.push_back(std::unique_ptr<FlatListNode>(flatList));
    ifStmt->children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("RPAREN")));
    ifStmt->children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("LBRACE")));
    // ifStmt->children.push_back(std::unique_ptr<FlatListNode>(flatList2));

    auto stmt = new NonTerminalNode();
    stmt->prodNumber = 5;
    stmt->nonTerminal = this->grammarDesc->GetNuts().at("stmt").get()->AsNonTerminal();
    auto t = debugMakeFixedTerminalNode("IDENTIFIER");
    t.value = "childe";
    stmt->children.push_back(std::make_unique<TerminalNode>(t));

    ifStmt->children.push_back(std::unique_ptr<NonTerminalNode>(stmt));
    ifStmt->children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("RBRACE")));
    ifStmt->prodNumber = 2;

    root = std::move(*ifStmt);

    std::cout << "ROOT TOSTRING " << root.ToString() << "\n";
}

void BeautyEngine::Repaint(sf::RenderWindow &window)
{
    auto chunkRoot = std::make_unique<RenderChunkList>(sf::Vector2f({0,0}));
    chunkRoot->children.push_back(std::make_unique<RenderChunkRectangle>(sf::RectangleShape({50,50}), sf::Color::Red, sf::Vector2f({100,0})));
    chunkRoot->children.push_back(std::make_unique<RenderChunkRectangle>(sf::RectangleShape({50,50}), sf::Color::Red, sf::Vector2f({100,100})));
    chunkRoot->children.push_back(std::make_unique<RenderChunkRectangle>(sf::RectangleShape({50,50}), sf::Color::Red, sf::Vector2f({100,200})));

    auto siemka = std::make_unique<RenderChunkText>("eeeee", sf::Vector2f({0, 0}));
    auto rec = std::make_unique<RenderChunkRectangle>(sf::RectangleShape(siemka->ComputeSize()), sf::Color::Green, sf::Vector2f({0, 0}));
    auto asd = std::make_unique<RenderChunkList>(sf::Vector2f({0,0}));
    asd->children.push_back(std::move(rec));
    asd->children.push_back(std::move(siemka));

    chunkRoot->children.push_back(std::move(asd));

    chunkRoot->Render(window, {0, 0});
    std::cout << "computed size" << chunkRoot->ComputeSize() << "\n";
}
