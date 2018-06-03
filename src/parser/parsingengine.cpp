#include <iostream> // debugging
#include <jsoncpp/json/json.h>
#include <fstream>

#include "parsingengine.hpp"

void ParsingEngine::ProcessKeypress(char32_t c) {
    auto tokens = lexingEngine->ProcessKeypress(c);
    for (auto& token : tokens) {
        std::cout << "CONSUMED " << token << "\n";
    }
}

ParsingEngine::ParsingEngine() {
    lexingEngine = std::make_unique<LexingEngine>();
}

Nut* ParsingEngine::makeAndGetNut(std::string lhs, bool terminal) {
    Nut* ret = getNut(lhs);
    if (ret != nullptr) {
        return ret;
    }
    std::cout << "making nut " << lhs << "\n";
    if (terminal) {
        nuts.push_back(std::make_unique<Terminal>(lhs));
    } else {
        nuts.push_back(std::make_unique<NonTerminal>(lhs));
    }
    ret = nuts.back().get();
    return ret;
}

Nut* ParsingEngine::getNut(std::string lhs) {
    for (auto& nut: nuts) {
        if(nut->lhs == lhs) {
            return nut.get();
        }
    }
    std::cout << "not found nut " << lhs << "\n";
    return nullptr;
}

bool validTokenName(std::string name) {
    for (char c : name) {
        if(!isupper(c)) {
            return false;
        }
    }
    return true;
}

void ParsingEngine::InitGrammar(std::string path) {
    std::ifstream ifs(path);
    Json::Reader reader;
	Json::Value obj;

	reader.parse(ifs, obj);

    std::vector<std::pair<std::string, std::string>> tokens;
    for (auto& token : obj["tokens"]) {
        auto tokenName = token.getMemberNames()[0];
        if(!validTokenName(tokenName)) {
            throw "invalid token name, all chars chould be uppercase";
        }
        auto tokenPattern = token[tokenName].asString();
        tokens.push_back({tokenName, tokenPattern});
        makeAndGetNut(tokenName, true); // ignore the return value, just create the nut
    }
    lexingEngine->Init(tokens);

    std::cout << "\n\nPRODUCTIONS\n";

    int currentProd = 0;
    for (auto& j : obj["productions"]) {
        auto name = j.getMemberNames()[0];
        NonTerminal* nonTerminal = static_cast<NonTerminal*>(makeAndGetNut(name, false));
        for (auto& prod : j[name]) {
            Production nonTermProd;
            nonTermProd.number = currentProd++;
            for(auto& n: prod) {
                nonTermProd.nuts.push_back(makeAndGetNut(n.asString(), validTokenName(n.asString())));
            }
            nonTerminal->productions.push_back(nonTermProd);
        }
    }

    for (auto& n : nuts) {
        n->dump();
    }
}