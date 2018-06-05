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

void ParsingEngine::InitGrammar(const std::string& path) {
    std::ifstream ifs(path);
    Json::Reader reader;
	Json::Value obj;

	reader.parse(ifs, obj);
    gdesc = GrammarDescription::FromJsonValue(obj);
    lexingEngine->Init(gdesc.GetTokenList());

    std::cout << "\n\nPRODUCTIONS\n";

    for (auto& p : gdesc.GetNuts()) {
        p.second->Dump();
    }
}