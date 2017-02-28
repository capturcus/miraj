#pragma once

#include <uchar.h>

#include "regex.h"

class LexingEngine {
public:
    LexingEngine();
    ~LexingEngine();
    void processKeypress(char32_t c);
private:
    std::vector<std::pair<RegEx*, std::string> > regexps;
};
