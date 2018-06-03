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

std::ostream & operator << (std::ostream &, const Token &);

///////////////// ABSTRACT MATCHER

class AbstractMatcher {
public:
    virtual StepResult Step(char32_t c) = 0;
    virtual StepResult StepBack() = 0;
    virtual Token* GetToken() = 0;
    virtual void Reset() = 0;
    virtual StepResult GetState() = 0;
    virtual void DebugPrintUnrejected() = 0;
    virtual bool InstantDetach() = 0;

    bool JustRejected();

    std::string value;
    std::vector<StepResult> history;
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
    bool InstantDetach();

    std::string pattern;
    int position = 0;
};

///////////////// NUMERAL LITERAL MATCHER
// matches only simple numbers like 3 or 123 (not -123 or 12.3)
class NumeralLiteralMatcher
    : public AbstractMatcher
{
public:
    NumeralLiteralMatcher();

    StepResult Step(char32_t c);
    StepResult StepBack();
    StepResult GetState();
    void Reset();
    Token* GetToken();
    void DebugPrintUnrejected();
    bool InstantDetach();
};

///////////////// IDENTIFIER MATCHER
// matches only simple literals ([a-zA-Z])
class IdentifierMatcher
    : public AbstractMatcher
{
public:
    IdentifierMatcher();

    StepResult Step(char32_t c);
    StepResult StepBack();
    StepResult GetState();
    void Reset();
    Token* GetToken();
    void DebugPrintUnrejected();
    bool InstantDetach();
};

///////////////// LEXING ENGINE

class ParsingEngine;

class LexingEngine {
public:
    LexingEngine();
    std::vector<Token> ProcessKeypress(char32_t c);

private:
    std::vector<std::unique_ptr<AbstractMatcher>> matchers;
};
