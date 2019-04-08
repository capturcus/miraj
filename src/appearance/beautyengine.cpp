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
    /*auto t = debugMakeFixedTerminalNode("IDENTIFIER");
    t.value = "childe";
    stmt->children.push_back(std::make_unique<TerminalNode>(t));*/

    ifStmt->children.push_back(std::unique_ptr<NonTerminalNode>(stmt));
    ifStmt->children.push_back(std::make_unique<TerminalNode>(debugMakeFixedTerminalNode("RBRACE")));
    ifStmt->prodNumber = 2;

    root = std::move(*ifStmt);

    std::cout << "ROOT TOSTRING " << root.ToString() << "\n";
}

void BeautyEngine::Repaint(sf::RenderWindow &window)
{
    auto chunkRoot = root.Render();
    chunkRoot->Render(window);
}