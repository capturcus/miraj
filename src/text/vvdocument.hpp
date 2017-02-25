#pragma once

#ifndef VVDOCUMENT_HPP
#define VVDOCUMENT_HPP

#include "document.hpp"

#include <vector>

class VVDocument : public Document
{
public:
    VVDocument();
    VVDocument(const char * sz);
    virtual ~VVDocument() {}

    virtual item at(position at) const override final;
    virtual position line(position at) const override final;
    virtual position column(position at) const override final;
    virtual position fromLineAndColumn(position line, position column) const override final;

    virtual ssize_t lineCount() const override final;

private:
    std::pair<position, position> findLineAndColumn(position at) const;

    std::vector<std::vector<item>> lines;
};

#endif // VVDOCUMENT_HPP

