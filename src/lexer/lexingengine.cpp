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
        matchers.push_back(std::make_unique<FixedTokenMatcher>(keyword));
    }
    std::ifstream punctuatorsFile("../data/punctuators.txt");
    std::string punctuator;
    while(punctuatorsFile >> punctuator) {
        matchers.push_back(std::make_unique<FixedTokenMatcher>(punctuator));
    }
    matchers.push_back(std::make_unique<IdentifierMatcher>());
    matchers.push_back(std::make_unique<NumeralLiteralMatcher>());
}

#define PENIS c==8

std::vector<Token> LexingEngine::ProcessKeypress(char32_t c) {
    std::vector<Token> ret;
    bool allRejected = true;
    std::cout << "===\n";
    for (auto& matcher : matchers) {
        StepResult result;
        if (PENIS) { // backspace
            result = matcher->StepBack();
        } else {
            result = matcher->Step(c);
        }
        if (result != REJECTED) {
            allRejected = false;
        }
        matcher->DebugPrintUnrejected();
    }

    if (allRejected) {
        std::vector<AbstractMatcher*> justRejected;
        for (auto& matcher : matchers) {
            if (matcher->JustRejected()) {
                justRejected.push_back(matcher.get());
            }
        }
        if (justRejected.size() > 0) {
            // temporary behavior? ask user?
            auto matcher = justRejected[0];
            matcher->StepBack(); // now is ACCEPTED
            Token* t = matcher->GetToken();
            ret.push_back(*t);
            delete t;
            for (auto& matcher : matchers) {
                matcher->Reset();
                matcher->Step(c);
            }
        }
    }

    // check instant detach
    bool noUndecided = true;
    std::vector<AbstractMatcher*> accepts;
    for (auto& matcher : matchers) {
        if (matcher->GetState() == UNDECIDED) {
            noUndecided = false;
            break;
        }
        if (matcher->GetState() == ACCEPTED) {
            accepts.push_back(matcher.get());
        }
    }
    if (noUndecided && accepts.size() == 1 && accepts[0]->InstantDetach()) {
        Token* t = accepts[0]->GetToken();
        ret.push_back(*t);
        delete t;
        for (auto& matcher : matchers) {
            matcher->Reset();
        }
    }
    return ret;
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
    if (history.back() != REJECTED && value.size() > 0) {
        value.pop_back();
    }
    history.pop_back();
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
    if (history.back() != REJECTED && value.size() > 0) {
        value.pop_back();
    }
    history.pop_back();
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