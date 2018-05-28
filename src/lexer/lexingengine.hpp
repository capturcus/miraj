#pragma once

#include <uchar.h>
#include <memory>
#include <vector>

enum StepResult {
    ACCEPTED,
    UNDECIDED,
    REJECTED
};

std::string StepResultToString(StepResult sr);

class Token {
public:
    Token(std::string t, std::string v);

    std::string type;
    std::string value;
};

///////////////// ABSTRACT MATCHER

class AbstractMatcher {
public:
    virtual StepResult Step(char32_t c) = 0;
    virtual StepResult StepBack() = 0;
    virtual Token* GetToken() = 0;
    virtual void Reset() = 0;
    virtual StepResult GetState() = 0;
    virtual void DebugPrintUnrejected() = 0;
};

///////////////// FIXED TOKEN MATCHER

class FixedTokenMatcher
    : public AbstractMatcher
{
public:
    FixedTokenMatcher(std::string p);

    StepResult Step(char32_t c);
    StepResult StepBack();
    StepResult GetState();
    void Reset();
    Token* GetToken();
    void DebugPrintUnrejected();

    std::string pattern;
    int position = 0;
    std::vector<StepResult> history;
};

///////////////// NUMERAL LITERAL MATCHER
// matches only simple numbers like 3 or 123 (not -123 or 12.3)
class NumeralLiteralMatcher
    : public AbstractMatcher
{
public:
    NumeralLiteralMatcher(std::string p);

    StepResult Step(char32_t c);
    StepResult StepBack();
    StepResult GetState();
    void Reset();
    Token* GetToken();
    void DebugPrintUnrejected();

    std::string value;
    std::vector<StepResult> history;
};

///////////////// LEXING ENGINE

class ParsingEngine;

class LexingEngine {
public:
    LexingEngine();
    void processKeypress(char32_t c);

    ParsingEngine* parsingEngine = nullptr;
private:
    std::vector<std::unique_ptr<AbstractMatcher>> tokens;
};
