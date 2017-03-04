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
    regex = new RegEx;
    reset();
}

MaybeToken::~MaybeToken() {
    delete regex;
}

StepResult MaybeToken::step(char c) {
    return regex->Step(c);
}

StepResult MaybeToken::stepBack() {
    return regex->StepBack();
}

void MaybeToken::reset() {
    regex->Compile(regexSrc);
}

///////////////// MAYBE PUNCTUATOR

MaybePunctuator::MaybePunctuator(const std::string& p)
    : punctuator(p)
{}

StepResult MaybePunctuator::step(char c){
    if (counter > punctuator.size()){
        counter++;
        return REJECTED;
    }
    if(punctuator[counter] != c) {
        counter++;
        return REJECTED;
    }
    counter++;
    if(counter == punctuator.size()) {
        return ACCEPTED;
    }
    return UNDECIDED;
}

StepResult MaybePunctuator::stepBack(){
}

void MaybePunctuator::reset(){
    counter = 0;
}

///////////////// LEXING ENGINE

LexingEngine::LexingEngine() {
    std::ifstream keywordsFile("../data/keywords.txt");
    std::string keyword;
    while(keywordsFile >> keyword) {
        tokens.push_back(new MaybeToken(keyword, keyword));
    }
    std::ifstream punctuatorsFile("../data/punctuators.txt");
    std::string punctuator;
    while(punctuatorsFile >> punctuator) {
        punctuators.push_back(new MaybePunctuator(punctuator));
    }
    tokens.push_back(new MaybeToken("IDENTIFIER", "([a-z])+"));
    tokens.push_back(new MaybeToken("NUMBER", "([0-9])+"));
}

LexingEngine::~LexingEngine() {
}

void LexingEngine::processKeypress(char32_t c) {
    for (MaybePunctuator* mp : punctuators) {
        StepResult result;
        if (c == 8) {
            result = mp->stepBack();
        } else {
            result = mp->step(c);
        }
        printf("%s\t%s\r\n", mp->punctuator.c_str(), printStepResult(result).c_str());
    }
}
