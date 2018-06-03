#include "grammar.hpp"
#include <iostream>
#include <memory>

using NutMap = std::map<std::string, std::unique_ptr<Nut>>;

void Terminal::Dump() const {
    std::cout << "TERM " << GetName() << " " << contents << "\n";
}

void NonTerminal::Dump() const {
    std::cout << "NONTERM " << GetName() << " [\n";
    for (auto& prod : productions) {
        std::cout << prod.number << ": ";
        for (auto& n : prod.nuts) {
            std::cout << n->GetName() << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "]\n";
}

static bool isValidTokenName(const std::string& name) {
    for (char c : name) {
        if(!isupper(c)) {
            return false;
        }
    }
    return true;
}

static void insertUniqueNut(
    NutMap& nuts,
    const std::string& name,
    std::unique_ptr<Nut> newNut
) {
    if (nuts.find(name) != nuts.end()) {
        // TODO: Better errors
        throw "conflicting names";
    }
    nuts.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(name),
        std::forward_as_tuple(std::move(newNut))
    );
}

static void parseTerminals(
    NutMap& nuts,
    const Json::Value& termsDesc
) {
    for (const auto& name : termsDesc.getMemberNames()) {
        if (!isValidTokenName(name)) {
            throw "invalid token name";
        }
        insertUniqueNut(
            nuts, name,
            std::make_unique<Terminal>(name, termsDesc[name].asString())
        );
    }
}

static Production parseProduction(
    NutMap& nuts,
    int productionNumber,
    const Json::Value& prodDesc
) {
    // TODO: Move member initialization into a constructor?
    Production prod;
    prod.number = productionNumber;
    prod.nuts.resize(prodDesc.size());

    for (Json::ArrayIndex i = 0; i < prodDesc.size(); i++) {
        const auto& key = prodDesc[i].asString();
        prod.nuts[(size_t)i] = nuts.at(key).get();
    }

    return prod;
}

static void parseNonterminals(NutMap& nuts, const Json::Value& nonTermsDesc) {
    const auto mbNames = nonTermsDesc.getMemberNames();

    // Allocate nonterminals
    for (const auto& name : mbNames) {
        insertUniqueNut(
            nuts, name,
            std::make_unique<NonTerminal>(name)
        );
    }

    // Initialize nonterminals with productions
    int prodNum = 0;
    for (const auto& name : mbNames) {
        NonTerminal* nt = static_cast<NonTerminal*>(nuts.at(name).get());
        const auto& prodsDesc = nonTermsDesc[name];
        for (Json::ArrayIndex i = 0; i < prodsDesc.size(); i++) {
            Production prod = parseProduction(nuts, prodNum, prodsDesc[i]);
            nt->AddProduction(std::move(prod));
            prodNum++;
        }
    }
}

GrammarDescription GrammarDescription::FromJsonValue(
    const Json::Value& description
) {
    NutMap nuts;
    parseTerminals(nuts, description["tokens"]);
    parseNonterminals(nuts, description["productions"]);

    return GrammarDescription(std::move(nuts));
}

std::vector<std::pair<std::string, std::string>> GrammarDescription::GetTokenList() const {
    std::vector<std::pair<std::string, std::string>> ret;
    for (const auto& p : nuts) {
        Terminal* terminal = p.second->AsTerminal();
        if (terminal != nullptr) {
            ret.push_back({terminal->GetName(), terminal->GetValue()});
        }
    }

    return ret;
}
