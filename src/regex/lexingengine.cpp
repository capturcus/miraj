#include <iostream>  //debugging
#include <vector>

#include "lexingengine.hpp"
#include "regex.h"

const std::string regexStrs[] = {
    "([0-9])+",
    "([a-z])+",
    "for",
};

const std::string tokenNames[] = {
    "NUMBER",
    "ID",
    "FOR",
};

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

LexingEngine::LexingEngine() {
    for (int i = 0; i < (sizeof(regexStrs)/sizeof(regexStrs[0])); i++) {
        RegEx* re = new RegEx;
        re->Compile(regexStrs[i]);
        regexps.push_back({re, tokenNames[i]});
    }
}

LexingEngine::~LexingEngine() {
    for (int i = 0; i < (sizeof(regexStrs)/sizeof(regexStrs[0])); i++) {
        delete regexps[i].first;
    }
}

void LexingEngine::processKeypress(char32_t c) {
    for (int i = 0; i < regexps.size(); i++) {
        StepResult result;
        if (c == 8) {
            result = regexps[i].first->StepBack();
        } else {
            result = regexps[i].first->Step(c);
        }
        printf("%s\t%s\r\n", regexps[i].second.c_str(), printStepResult(result).c_str());
    }
}
