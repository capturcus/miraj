#pragma once

#include <uchar.h>

#include "regex.h"

class MaybeRegex {
public:
    virtual StepResult step(char c) = 0;
    virtual StepResult stepBack() = 0;
    virtual void reset() = 0;
};

class MaybeToken
    : public MaybeRegex
{
public:
    MaybeToken(const std::string&, const std::string&);
    ~MaybeToken();
    StepResult step(char c);
    StepResult stepBack();
    void reset();

    const std::string name;
    const std::string regexSrc;
    RegEx* regex;
};

class MaybePunctuator
    : public MaybeRegex
{
public:
    MaybePunctuator(const std::string&);
    StepResult step(char c);
    StepResult stepBack();
    void reset();

    const std::string punctuator;
private:
    int counter = 0;
};

class LexingEngine {
public:
    LexingEngine();
    ~LexingEngine();
    void processKeypress(char32_t c);
private:
    std::vector<MaybeToken*> tokens;
    std::vector<MaybePunctuator*> punctuators;
};
