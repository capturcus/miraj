#include <iostream>  //debugging

#include <vector>
#include <fstream>

#include "lexingengine.hpp"
#include "../parser/parsingengine.hpp"

std::string StepResultToString(StepResult sr) {
    switch (sr) {
        case ACCEPTED:
        return "ACCEPTED";
        case UNDECIDED:
        return "UNDECIDED";
        case REJECTED:
        return "REJECTED";
    }
}

///////////////// Token

Token::Token(std::string t, std::string v)
    : type(t), value(v)
{}

///////////////// FixedTokenMatcher

FixedTokenMatcher::FixedTokenMatcher(std::string p)
    : pattern(p)
{
    history.push_back(UNDECIDED);
}

StepResult FixedTokenMatcher::Step(char32_t c){
    if (position > pattern.size()){
        position++;
        history.push_back(REJECTED);
        return REJECTED;
    }
    if(pattern[position] != c) {
        position++;
        history.push_back(REJECTED);
        return REJECTED;
    }
    position++; // from now on we know the char matches
    for (StepResult sr : history) {
        if (sr == REJECTED) {
            history.push_back(REJECTED);
            return REJECTED;
        }
    }
    if(position == pattern.size()) {
        history.push_back(ACCEPTED);
        return ACCEPTED;
    }
    history.push_back(UNDECIDED);
    return UNDECIDED;
}

StepResult FixedTokenMatcher::StepBack(){
    position--;
    history.pop_back();
    return history.back();
}

void FixedTokenMatcher::Reset(){
    position = 0;
    history.clear();
    history.push_back(UNDECIDED);
}

Token* FixedTokenMatcher::GetToken(){
    if (history.back() == ACCEPTED) {
        return new Token(pattern, std::string(""));
    } else {
        std::cout << "TRYING TO GET NOT ACCEPTED TOKEN!\n";
        std::cout << StepResultToString(history.back()) << "\n";
        return nullptr;
    }
}

StepResult FixedTokenMatcher::GetState(){
    return history.back();
}

void FixedTokenMatcher::DebugPrintUnrejected(){
    if (history.back() != REJECTED) {
        printf("%s %s\n", pattern.c_str(), StepResultToString(history.back()).c_str());
    }
}

///////////////// LEXING ENGINE

LexingEngine::LexingEngine() {
    std::ifstream keywordsFile("../data/keywords.txt");
    std::string keyword;
    while(keywordsFile >> keyword) {
        tokens.push_back(std::make_unique<FixedTokenMatcher>(keyword));
    }
    std::ifstream punctuatorsFile("../data/punctuators.txt");
    std::string punctuator;
    while(punctuatorsFile >> punctuator) {
        tokens.push_back(std::make_unique<FixedTokenMatcher>(punctuator));
    }
    // tokens.push_back(std::make_unique<MaybeToken>("IDENTIFIER", "([a-z])+"));
    // tokens.push_back(std::make_unique<MaybeToken>("NUMBER", "([0-9])+"));
}

#define PENIS c==8

void LexingEngine::processKeypress(char32_t c) {
    std::vector<AbstractMatcher*> acceptingTokens;
    bool undecidedPresent = false;
    std::cout << "===\n";
    for (auto& token : tokens) {
        StepResult result;
        if (PENIS) { // backspace
            result = token->StepBack();
        } else {
            result = token->Step(c);
        }
        if (result == UNDECIDED) {
            undecidedPresent = true;
        }
        token->DebugPrintUnrejected();

        if (token->GetState() == ACCEPTED) {
            acceptingTokens.push_back(token.get());
        }
    }
    // if there are some accepting tokens, take the first one, because they should be
    // sorted in such an order that it's gonna be the correct one (a keyword or punctuation if possible)
    if (!undecidedPresent && acceptingTokens.size() > 0) {
        Token* t = acceptingTokens[0]->GetToken();
        if (t == nullptr) {
            std::cout << "COULD NOT GET TOKEN\n";
        }
        parsingEngine->consumeToken(t);
        for (auto& token : tokens) {
            token->Reset();
        }
    }
}
