#pragma once

#include <uchar.h>
#include <memory>

#include "regex.h"

class MaybeRegex {
public:
    virtual StepResult step(char32_t c) = 0;
    virtual StepResult stepBack() = 0;
    virtual void reset() = 0;
    virtual const char* getNamePtr() = 0;
};

class MaybeToken
    : public MaybeRegex
{
public:
    MaybeToken(const std::string&, const std::string&);
    StepResult step(char32_t c);
    StepResult stepBack();
    void reset();
    virtual const char* getNamePtr();

    const std::string name;
    const std::string regexSrc;
    RegEx regex;
};

class MaybePunctuator
    : public MaybeRegex
{
public:
    MaybePunctuator(const std::string&);
    StepResult step(char32_t c);
    StepResult stepBack();
    void reset();
    virtual const char* getNamePtr();

    const std::string punctuator;
private:
    int counter = 0;
    std::vector<StepResult> history;
};

class LexingEngine {
public:
    LexingEngine();
    ~LexingEngine();
    void processKeypress(char32_t c);
private:
    std::vector<std::unique_ptr<MaybeRegex>> tokens;
};
