#include <iostream>  //debugging

#include <vector>
#include <fstream>

#include "lexingengine.hpp"
#include "regex.h"

const std::string printStepResult(StepResult result) {
    switch(result) {
        case ACCEPTED:
            return "ACCEPTED";
        case UNDECIDED:
            return ("UNDECIDED");
        case REJECTED:
            return ("REJECTED");
    }
}

///////////////// MAYBE TOKEN

MaybeToken::MaybeToken(const std::string& name_, const std::string& regex_)
    : name(name_), regexSrc(regex_)
{
    reset();
}

StepResult MaybeToken::step(char32_t c) {
    StepResult res = regex.Step(c);
    history.push_back({res, c});
    return res;
}

StepResult MaybeToken::stepBack() {
    history.pop_back();
    return regex.StepBack();
}

void MaybeToken::reset() {
    history.clear();
    history.push_back({UNDECIDED, '\0'});
    regex.Compile(regexSrc);
}

const char* MaybeToken::getNamePtr() {
    return name.c_str();
}

StepResult MaybeToken::lastResult() {
    return history.back().first;
}

std::string MaybeToken::getValue() {
    std::string value;
    for (auto& p : history) {
        if (p.second != '\0' && p.first != REJECTED) {
            value += p.second;
        }
    }
    std::cout << value << "\n";
    return value;
}

///////////////// MAYBE PUNCTUATOR

MaybePunctuator::MaybePunctuator(const std::string& p)
    : punctuator(p)
{
    reset();
}

StepResult MaybePunctuator::step(char32_t c){
    if (counter > punctuator.size()){
        counter++;
        history.push_back(REJECTED);
        return REJECTED;
    }
    if(punctuator[counter] != c) {
        counter++;
        history.push_back(REJECTED);
        return REJECTED;
    }
    counter++; // from now on we know the char matches
    for (StepResult sr : history) {
        if (sr == REJECTED) {
            history.push_back(REJECTED);
            return REJECTED;
        }
    }
    if(counter == punctuator.size()) {
        history.push_back(ACCEPTED);
        return ACCEPTED;
    }
    history.push_back(UNDECIDED);
    return UNDECIDED;
}

StepResult MaybePunctuator::stepBack(){
    counter--;
    history.pop_back();
    return history.back();
}

void MaybePunctuator::reset(){
    counter = 0;
    history.clear();
    history.push_back(UNDECIDED);
}

const char* MaybePunctuator::getNamePtr() {
    return punctuator.c_str();
}

StepResult MaybePunctuator::lastResult() {
    return history.back();
}

std::string MaybePunctuator::getValue() {
    return punctuator;
}

///////////////// LEXING ENGINE

LexingEngine::LexingEngine() {
    std::ifstream keywordsFile("../data/keywords.txt");
    std::string keyword;
    while(keywordsFile >> keyword) {
        tokens.push_back(std::make_unique<MaybeToken>(keyword, keyword));
    }
    std::ifstream punctuatorsFile("../data/punctuators.txt");
    std::string punctuator;
    while(punctuatorsFile >> punctuator) {
        tokens.push_back(std::make_unique<MaybePunctuator>(punctuator));
    }
    tokens.push_back(std::make_unique<MaybeToken>("IDENTIFIER", "([a-z])+"));
    tokens.push_back(std::make_unique<MaybeToken>("NUMBER", "([0-9])+"));
}

void LexingEngine::processKeypress(char32_t c) {
    std::cout << "\n";
    std::vector<MaybeRegex*> acceptingTokens;
    for (auto& token : tokens) {
        if (token->lastResult() == ACCEPTED) {
            acceptingTokens.push_back(token.get());
        }
    }
    bool allRejected = true;
    for (auto& token : tokens) {
        StepResult result;
        if (c == 8) {
            result = token->stepBack();
        } else {
            result = token->step(c);
        }
        if (result != REJECTED) {
            allRejected = false;
            printf("%s\t%s\r\n", token->getNamePtr(), printStepResult(result).c_str());
        }
    }
    if (allRejected && acceptingTokens.size() > 0) {
        parsingEngine->consumeToken({
                std::string(acceptingTokens[0]->getNamePtr()),
                std::string(acceptingTokens[0]->getValue())
                });
        for (auto& token : tokens) {
            token.reset();
            StepResult result;
            if (c == 8) {
                token->stepBack();
            } else {
                token->step(c);
            }
        }
    }
}
