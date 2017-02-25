#include "vvdocument.hpp"

VVDocument::VVDocument() : VVDocument("") {}

VVDocument::VVDocument(const char * sz)
{
    // TODO: Different line endings handling
    // TODO: Unicode...
    std::vector<item> line;

    while (*sz != '\0') {
        if (*sz == eol) {
            lines.emplace_back(std::move(line));
            line.clear();
        }
        else {
            line.push_back((char32_t)*sz);
        }
        ++sz;
    }

    lines.emplace_back(std::move(line));
}

Document::item VVDocument::at(position at) const
{
    const auto p = findLineAndColumn(at);
    if (p.second == lines[p.first].size()) {
        return eol;
    }

    return lines[p.first][p.second];
}

Document::position VVDocument::line(position at) const
{
    return findLineAndColumn(at).first;
}

Document::position VVDocument::column(position at) const
{
    return findLineAndColumn(at).second;
}

Document::position VVDocument::fromLineAndColumn(position line, position column) const
{
    if (line < 0) {
        line = 0;
    }

    if (line >= lines.size()) {
        line = lines.size() - 1;
    }

    if (column < 0) {
        column = 0;
    }

    position ret = 0;
    for (position l = 0; l < line; ++l) {
        ret += lines[l].size() + 1;
    }

    return ret + std::min(column, (position)(lines[line].size() + 1));
}

ssize_t VVDocument::lineCount() const
{
    return lines.size();
}

std::pair<Document::position, Document::position> VVDocument::findLineAndColumn(position at) const
{
    position lineNumber = 0;
    for (const auto & line : lines) {
        if (line.size() < at) {
            at -= line.size() + 1;
            ++lineNumber;
        }
        else {
            return { lineNumber, at };
        }
    }

    return { lineNumber - 1, 0 };
}

