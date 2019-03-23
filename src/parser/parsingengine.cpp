#include <iostream> // debugging
#ifdef _WIN32
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif
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

    // std::cout << "\n\nPRODUCTIONS\n";

    /*for (auto& p : gdesc.GetNuts()) {
        p.second->Dump();
    }*/
}