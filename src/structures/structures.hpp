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

    virtual std::string ToString() = 0;
};

class TerminalNode 
    : public DisplayNode
{
public:
    virtual TerminalNode* AsTerminalNode() override {
        return this;
    }

    std::string ToString() override;

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

    std::string ToString() override;

    FlatList* flatList = nullptr;
    std::vector<std::unique_ptr<DisplayNode>> children;
    Terminal* separator;
};