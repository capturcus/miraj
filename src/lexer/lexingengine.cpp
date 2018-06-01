#include <iostream>  //debugging

#include <vector>
#include <fstream>

#include "lexingengine.hpp"
#include "../parser/parsingengine.hpp"

std::ostream & operator << (std::ostream& os, const Token& t) {
    os << "(" << t.type << "," << t.value << ")";
}

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

///////////////// AbstractMatcher

bool AbstractMatcher::JustRejected() {
    if (history.size() < 2) {
        return false;
    }
    size_t s = history.size();
    return history[s-1] == REJECTED && history[s-2] == ACCEPTED;
}

///////////////// FixedTokenMatcher

FixedTokenMatcher::FixedTokenMatcher(std::string p)
    : pattern(p)
{
    history.push_back(UNDECIDED);
}

bool FixedTokenMatcher::InstantDetach() {
    return true;
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

Token* FixedTokenMatcher::GetToken() {
    // if (history.back() == ACCEPTED) {
    return new Token(pattern, std::string(""));
    // } else {
    //     std::cout << "TRYING TO GET NOT ACCEPTED FIXED TOKEN " << pattern << "\n";
    //     std::cout << StepResultToString(history.back()) << "\n";
    //     return nullptr;
    // }
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
    tokens.push_back(std::make_unique<IdentifierMatcher>());
    tokens.push_back(std::make_unique<NumeralLiteralMatcher>());
}

std::vector<AbstractMatcher*> LexingEngine::GetAcceptingTokens() {
    std::vector<AbstractMatcher*> acceptingTokens;
    for (auto& token : tokens) {
        if (token->GetState() == ACCEPTED) {
            acceptingTokens.push_back(token.get());
        }
    }
    return acceptingTokens;
}

#define PENIS c==8

void LexingEngine::ProcessKeypress(char32_t c) {
    
    auto acceptingTokens = this->GetAcceptingTokens();

    bool allRejected = true;
    std::cout << "===\n";
    for (auto& token : tokens) {
        StepResult result;
        if (PENIS) { // backspace
            result = token->StepBack();
        } else {
            result = token->Step(c);
        }
        if (result != REJECTED) {
            allRejected = false;
        }
        token->DebugPrintUnrejected();
    }

    // check instant detach
    auto acceptingTokens2 = this->GetAcceptingTokens();
    bool instantDetach = false;
    for (auto& token : acceptingTokens2) {
        if (token->InstantDetach()) {
            auto t = token->GetToken();
            if(instantDetach) {
                std::cout << "DOUBLE INSTANT DETACH, VERY WRONG!!! " << *t << " EXITING\n";
                exit(1);
            }
            parsingEngine->consumeToken(*t);
            instantDetach = true;
            delete t;
        }
    }
    if (instantDetach) {
        for (auto& token : tokens) {
            token->Reset();
        }
        return;
    }

    if (allRejected) {
        // all matchers rejected, check whether before stepping the character there was a viable matcher
        if (acceptingTokens.size() == 0) {
            // nothing accepted, reject character
            std::cout << "REJECT CHARACTER\n";
        }
        if (acceptingTokens.size() == 1) {
            // there is exactly one, emit it
            Token* t = acceptingTokens[0]->GetToken();
            if (t == nullptr) {
                std::cout << "COULD NOT GET TOKEN\n";
            }
            parsingEngine->consumeToken(*t);
            delete t;

            // one token accepted and emitted. Reset tokens, process current character. Possibly emit another token on instant detach.
            instantDetach = false;
            for (auto& token : tokens) {
                token->Reset();
                token->Step(c);
                if (token->GetState() == ACCEPTED && token->InstantDetach()) {
                    if(instantDetach) {
                        std::cout << "DOUBLE INSTANT DETACH 2, VERY WRONG!!! EXITING\n";
                        exit(1);
                    }
                    instantDetach = true;
                    // emit!
                    auto t = token->GetToken();
                    parsingEngine->consumeToken(*t);
                    delete t;
                }
            }
            if (instantDetach) {
                for (auto& token : tokens) {
                    token->Reset();
                }
            }
        }
        if (acceptingTokens.size() > 1) {
            std::cout << "REJECT CHARACTER AND MAKE USER CHOOSE\n";
        }
    }
}

///////////////// NUMERAL LITERAL MATCHER

NumeralLiteralMatcher::NumeralLiteralMatcher() {
    history.push_back(UNDECIDED);
}

bool NumeralLiteralMatcher::InstantDetach() {
    return false;
}

StepResult NumeralLiteralMatcher::Step(char32_t c) {
    if (isdigit(c) && history.back() != REJECTED) {
        value.push_back(c);
        history.push_back(ACCEPTED);
        return ACCEPTED;
    } else {
        history.push_back(REJECTED);
        return REJECTED;
    }
}

StepResult NumeralLiteralMatcher::StepBack() {
    history.pop_back();
    if (history.back() != REJECTED && value.size() > 0) {
        value.pop_back();
    }
    return history.back();
}

StepResult NumeralLiteralMatcher::GetState() {
    return history.back();
}

void NumeralLiteralMatcher::Reset() {
    value = "";
    history.clear();
    value.clear();
    history.push_back(UNDECIDED);
}

Token* NumeralLiteralMatcher::GetToken() {
    // if (history.back() == ACCEPTED) {
    return new Token("numeralliteral", value);
    // } else {
    //     std::cout << "TRYING TO GET NOT ACCEPTED NUMERAL TOKEN! " << value << "\n";
    //     std::cout << StepResultToString(history.back()) << "\n";
    //     return nullptr;
    // }
}

void NumeralLiteralMatcher::DebugPrintUnrejected() {
    if (history.back() != REJECTED) {
        printf("NUMERAL %s %s\n", value.c_str(), StepResultToString(history.back()).c_str());
    }
}

///////////////// IDENTIFIER MATCHER

IdentifierMatcher::IdentifierMatcher() {
    history.push_back(UNDECIDED);
}

bool IdentifierMatcher::InstantDetach() {
    return false;
}

StepResult IdentifierMatcher::Step(char32_t c) {
    if (isalpha(c) && history.back() != REJECTED) {
        value.push_back(c);
        history.push_back(ACCEPTED);
        return ACCEPTED;
    } else {
        history.push_back(REJECTED);
        return REJECTED;
    }
}

StepResult IdentifierMatcher::StepBack() {
    history.pop_back();
    if (history.back() != REJECTED && value.size() > 0) {
        value.pop_back();
    }
    return history.back();
}

StepResult IdentifierMatcher::GetState() {
    return history.back();
}

void IdentifierMatcher::Reset() {
    value = "";
    history.clear();
    value.clear();
    history.push_back(UNDECIDED);
}

Token* IdentifierMatcher::GetToken() {
    // if (history.back() == ACCEPTED) {
    return new Token("identifier", value);
    // } else {
    //     std::cout << "TRYING TO GET NOT ACCEPTED IDENTIFIER TOKEN! " << value << "\n";
    //     std::cout << StepResultToString(history.back()) << "\n";
    //     return nullptr;
    // }
}

void IdentifierMatcher::DebugPrintUnrejected() {
    if (history.back() != REJECTED) {
        printf("IDENTIFIER %s %s\n", value.c_str(), StepResultToString(history.back()).c_str());
    }
}