#include "grammar.hpp"
#include <iostream>
#include <memory>

// using NutMap = std::map<std::string, std::unique_ptr<Nut>>;

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
        if (prod.displayFormat != "") {
            std::cout << "(display: " << prod.displayFormat << ")";
        }
        std::cout << "|\n";
    }
    std::cout << "]\n";
}

void FlatList::Dump() const {
    std::cout << "FLATLIST " << GetName() << "(" << child->GetName() << ", " << separator->GetName() << ")\n";
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

    // add built-ins: IDENTIFIER and NUMERAL
    insertUniqueNut(
            nuts, "IDENTIFIER",
            std::make_unique<Terminal>("IDENTIFIER", "<identifier>")
        );
    insertUniqueNut(
        nuts, "NUMERAL",
        std::make_unique<Terminal>("NUMERAL", "<numeral>")
    );
}

static Production parseProduction(
    NutMap& nuts,
    int productionNumber,
    const Json::Value& completeProdDesc
) {
    const auto& prodDesc = completeProdDesc["production"];

    // TODO: Move member initialization into a constructor?
    Production prod;
    prod.number = productionNumber;
    prod.nuts.resize(prodDesc.size());

    for (Json::ArrayIndex i = 0; i < prodDesc.size(); i++) {
        const auto& key = prodDesc[i].asString();
        prod.nuts[(size_t)i] = nuts.at(key).get();
    }

    if (completeProdDesc.isMember("display")) {
        prod.displayFormat = completeProdDesc["display"].asString();
    }

    return prod;
}

static void parseNonterminals(NutMap& nuts, const Json::Value& nonTermsDesc) {
    const auto mbNames = nonTermsDesc.getMemberNames();

    // Allocate nonterminals
    for (const auto& name : mbNames) {
        const auto& nutDesc = nonTermsDesc[name];
        if (nutDesc.type() == Json::objectValue) {
            // object means it's a FlatList
            insertUniqueNut(
                nuts, name,
                std::make_unique<FlatList>(name)
            );
        } else if (nutDesc.type() == Json::arrayValue) {
            // array means it's a NonTerminal
            insertUniqueNut(
                nuts, name,
                std::make_unique<NonTerminal>(name)
            );
        }
    }

    // Initialize nonterminals with productions
    int prodNum = 0;
    for (const auto& name : mbNames) {
        const auto& nutDesc = nonTermsDesc[name];
        if (nutDesc.type() == Json::objectValue) {
            // FlatList
            FlatList* fl = static_cast<FlatList*>(nuts.at(name).get());
            const std::string& child = nutDesc["child"].asString();
            fl->child = nuts.at(child).get();
            fl->separator = nuts.at(nutDesc["separator"].asString())->AsTerminal();
        } else if (nutDesc.type() == Json::arrayValue) {
            // NonTerminal
            NonTerminal* nt = static_cast<NonTerminal*>(nuts.at(name).get());
            for (Json::ArrayIndex i = 0; i < nutDesc.size(); i++) {
                const Json::Value& prodDesc = nutDesc[i];
                Production prod = parseProduction(nuts, prodNum, prodDesc);
                nt->AddProduction(std::move(prod));
                prodNum++;
            }
        }
    }
}

GrammarDescription GrammarDescription::FromJsonValue(
    const Json::Value& description
) {
    NutMap nuts;
    parseTerminals(nuts, description["terminals"]);
    parseNonterminals(nuts, description["nonterminals"]);

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
