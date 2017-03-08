#pragma once

#include <uchar.h>
#include <memory>

#include "regex.h"

#include "../parser/parsingengine.hpp"

class MaybeRegex {
public:
    virtual StepResult step(char32_t c) = 0;
    virtual StepResult stepBack() = 0;
    virtual void reset() = 0;
    virtual const char* getNamePtr() = 0;
    virtual StepResult lastResult() = 0;
    virtual std::string getValue() = 0;
};

///////////////// MAYBE TOKEN

class MaybeToken
    : public MaybeRegex
{
public:
    MaybeToken(const std::string&, const std::string&);
    StepResult step(char32_t c);
    StepResult stepBack();
    void reset();
    virtual const char* getNamePtr();
    StepResult lastResult();
    std::string getValue();

    const std::string name;
    const std::string regexSrc;
    RegEx regex;
    std::vector<std::pair<StepResult, char>> history;
};

///////////////// MAYBE PUNCTUATOR

class MaybePunctuator
    : public MaybeRegex
{
public:
    MaybePunctuator(const std::string&);
    StepResult step(char32_t c);
    StepResult stepBack();
    void reset();
    virtual const char* getNamePtr();
    StepResult lastResult();
    std::string getValue();

    const std::string punctuator;
private:
    int counter = 0;
    std::vector<StepResult> history;
};

///////////////// LEXING ENGINE

class LexingEngine {
public:
    LexingEngine();
    void processKeypress(char32_t c);

    ParsingEngine* parsingEngine = nullptr;
private:
    std::vector<std::unique_ptr<MaybeRegex>> tokens;
};
