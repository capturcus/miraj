#pragma once

#include "../parser/grammar.hpp"

class TerminalNode;
class NonTerminalNode;
class FlatListNode;

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
};

class TerminalNode 
    : public DisplayNode
{
public:
    virtual TerminalNode* AsTerminalNode() override {
        return this;
    }

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

    NonTerminal* nonTerminal  = nullptr;
    std::vector<std::unique_ptr<DisplayNode>> children;
};

class FlatListNode
    : public DisplayNode
{
public:
    virtual FlatListNode* AsFlatListNode() override  {
        return this;
    }

    FlatList* flatList = nullptr;
    std::vector<std::unique_ptr<DisplayNode>> children;
    Terminal* separator;
};