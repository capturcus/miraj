#pragma once
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#ifdef _WIN32
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif

class Nut;
class Terminal;
class NonTerminal;
class FlatList;

using NutMap = std::map<std::string, std::unique_ptr<Nut>>;

struct Production {
    int number = -2;
    std::vector<Nut*> nuts;
    std::string displayFormat;
};

class Nut {
private:
    std::string name;

public:
    Nut(std::string name) : name(std::move(name)) {}
    virtual ~Nut() {}

    inline const std::string& GetName() const {
        return name;
    }

    virtual Terminal* AsTerminal() {
        return nullptr;
    }

    virtual NonTerminal* AsNonTerminal() {
        return nullptr;
    }

    virtual FlatList* AsFlatList() {
        return nullptr;
    }

    virtual void Dump() const = 0;
};

class Terminal final
    : public Nut
{
private:
    std::string contents;

public:
    Terminal(std::string name, std::string contents)
            : Nut(name)
            , contents(contents) {
    }

    inline std::string GetValue() const {
        return std::string(contents);
    }

    virtual Terminal* AsTerminal() override {
        return this;
    }

    void Dump() const override;
};

class NonTerminal final
    : public Nut
{
private:
    std::vector<Production> productions;

public:
    NonTerminal(std::string name)
            : Nut(name) {
    }

    virtual NonTerminal* AsNonTerminal() override {
        return this;
    }

    inline const std::vector<Production> GetProductions() const {
        return productions;
    }

    inline void AddProduction(Production prod) {
        productions.emplace_back(std::move(prod));
    }

    void Dump() const override;
};

class FlatList final
    : public Nut
{
public:
    Nut* child;
    Terminal* separator;
    FlatList(std::string name)
            : Nut(name)
            {
    }

    virtual FlatList* AsFlatList() override {
        return this;
    }

    void Dump() const override;
};

struct GrammarDescription {
private:
    inline GrammarDescription(std::map<std::string, std::unique_ptr<Nut>> nuts)
        : nuts(std::move(nuts))
    {}

    std::map<std::string, std::unique_ptr<Nut>> nuts;

public:
    GrammarDescription() {};

    static GrammarDescription FromJsonValue(const Json::Value& description);

    std::vector<std::pair<std::string, std::string>> GetTokenList() const;
    inline const NutMap& GetNuts() const {
        return nuts;
    }
};
