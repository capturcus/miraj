#pragma once

#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <cstdint>
#include <uchar.h>
#include <memory>

class Document;

class Document
{
public:
    using item = char32_t;
    using position = int64_t;

    static const item eof = 0xFFFFFFFF;
    static const item eol = '\n';
    static const position eol_pos = -1;

    virtual ~Document() {}

    virtual item at(position at) const = 0;
    virtual position line(position at) const = 0;
    virtual position column(position at) const = 0;
    virtual position fromLineAndColumn(position line, position column) const = 0;

    virtual ssize_t lineCount() const = 0;
};

#endif // DOCUMENT_HPP

