#include "structures.hpp"

const int SPACE_WIDTH = 10;
const int TAB_WIDTH = SPACE_WIDTH*4;

sf::Text makeText(std::string t);

/////////////// DISPLAY NODES

// std::string DisplayNode::ToString() {
//     return "<display node>";
// }

// std::unique_ptr<RenderChunk> DisplayNode::Render() {
//     return nullptr;
// }

std::string TerminalNode::ToString()
{
    return this->value;
}

std::string NonTerminalNode::ToString()
{
    std::string ret;
    for (auto &child : this->children)
    {
        ret += child->ToString() + " ";
    }
    if (ret.length() > 0)
    {
        ret.pop_back();
    }
    return ret;
}

std::string FlatListNode::ToString()
{
    std::string ret;
    for (auto &child : this->children)
    {
        ret += child->ToString() + flatList->separator->GetValue();
    }
    if (ret.length() > 0)
    {
        ret.pop_back(); // fixme what if the separator is longer than 1 char
    }
    return ret;
}

std::unique_ptr<RenderChunk> TerminalNode::Render()
{
    auto ret = std::make_unique<RenderChunkText>(this->value, sf::Vector2f());
    return std::move(ret);
    // return std::unique_ptr<RenderChunkRectangle>(new RenderChunkRectangle(sf::RectangleShape({2,2}), sf::Color::Red, {50,50}));
}

std::unique_ptr<RenderChunk> NonTerminalNode::Render()
{
    // find the production that the current node is in
    auto currentProduction = myProduction();
    if (currentProduction.number == -2)
    {
        std::cout << "ERROR currentProduction " << this->prodNumber << " not found\n";
        exit(1);
    }

    RenderChunkList *ret = new RenderChunkList({0, 0});
    sf::Vector2f currentOffset;

    size_t displayCharCount = 0;
    size_t childCount = 0;

    // if empty just display first child
    if (currentProduction.displayFormat == "")
    {
        currentProduction.displayFormat = "@";
    }

    std::cout << "DISPLAY FORMAT " << currentProduction.displayFormat << "\n";

    bool tabbed = false;
    while (displayCharCount < currentProduction.displayFormat.size())
    {
        auto ch = currentProduction.displayFormat[displayCharCount];
        if (ch == '@' || ch == '%')
        {
            std::cout << "DISCOVERED NUT " << childCount << " " << "\n";
            auto& child = this->children[childCount];
            auto chunk = child->Render();

            chunk->position = currentOffset;
            if (tabbed) {
                chunk->position.x += TAB_WIDTH;
                tabbed = false;
            }

            if (ch == '%')
                currentOffset += chunk->ComputeSize();
            if (ch == '@')
                currentOffset += {chunk->ComputeSize().x, 0};
            ret->children.push_back(std::move(chunk));
            childCount++;
        }
        else if (ch == ' ')
        {
            std::cout << "SPACE\n";
            currentOffset.x += SPACE_WIDTH;
        }
        else if (ch == '\t')
        {
            std::cout << "TAB\n";
            tabbed = true;
        }
        else if (ch == '\n')
        {
            std::cout << "NEWLINE\n";
            currentOffset.x = 0;
            currentOffset.y += FONT_SIZE;
        }
        else
        {
            std::cout << "wrong char in display format: " << ch << "\n";
        }
        displayCharCount++;
    }

    return std::unique_ptr<RenderChunk>(ret);
    // return std::unique_ptr<RenderChunkRectangle>(new RenderChunkRectangle(sf::RectangleShape({2,2}), sf::Color::Red, {50,50}));
}

std::unique_ptr<RenderChunk> FlatListNode::Render()
{
    auto ret = std::make_unique<RenderChunkList>(sf::Vector2f());

    bool separatorNewline = int(this->flatList->separator->GetValue()[0]) == 10;

    sf::Vector2f currentOffset;
    for (size_t i = 0; i < children.size(); i++)
    {
        auto& child = children[i];
        auto chunk = child->Render();
        chunk->position = currentOffset;
        if (separatorNewline) {
            currentOffset += {0, FONT_SIZE};
        } else {
            currentOffset += {chunk->ComputeSize().x, 0};
        }
        ret->children.push_back(std::move(chunk));
        // also push the separator if not at the end
        if (i != children.size()-1) {
            // not last child
            if (!separatorNewline)
                currentOffset += {SPACE_WIDTH, 0};
            auto text = std::make_unique<RenderChunkText>(this->flatList->separator->GetValue(), currentOffset);
            if (!separatorNewline) {
                auto textWidth = text->ComputeSize().x;
                currentOffset += {textWidth + SPACE_WIDTH, 0};
            }

            ret->children.push_back(std::move(text));
        }
    }

    return std::move(ret);
    // return std::unique_ptr<RenderChunkRectangle>(new RenderChunkRectangle(sf::RectangleShape({2,2}), sf::Color::Red, {50,50}));
}

Production NonTerminalNode::myProduction() {
    auto productions = this->nonTerminal->GetProductions();
    Production currentProduction;
    for (auto &p : productions)
    {
        if (p.number == this->prodNumber)
        {
            currentProduction = p;
            break;
        }
    }
    return currentProduction;
}

sf::Vector2f ensureCover(sf::Vector2f coveringRect, sf::FloatRect coveredRect)
{
    sf::Vector2f ret;
    ret.x = std::max(coveredRect.width + coveredRect.left, coveringRect.x);
    ret.y = std::max(coveredRect.height + coveredRect.top, coveringRect.y);
    // for now ignore top and left
    return ret;
}

RenderChunk::RenderChunk(sf::Vector2f pos) 
    : position(pos)
{}

sf::Vector2f vecMax(sf::Vector2f a, sf::Vector2f b) {
    return {std::max(a.x, b.x), std::max(a.y, b.y)};
}

RenderChunkRectangle::RenderChunkRectangle(sf::RectangleShape rect_, sf::Color c, sf::Vector2f offset_)
    : RenderChunk(offset_), rect(rect_)
{
    rect.setFillColor(c);
}

sf::Vector2f RenderChunkRectangle::ComputeSize() {
    return { rect.getLocalBounds().width, rect.getLocalBounds().height };
}

void RenderChunkRectangle::Render(sf::RenderWindow& window, sf::Vector2f offset) {
    rect.setPosition(offset);
    window.draw(rect);
}

RenderChunkList::RenderChunkList(sf::Vector2f pos)
    : RenderChunk(pos)
{}

sf::Vector2f RenderChunkList::ComputeSize() {
    sf::Vector2f max = {0, 0};
    for (auto& c : children) {
        max = vecMax(max, c->ComputeSize() + c->position);
    }
    return max;
}

void RenderChunkList::Render(sf::RenderWindow& window, sf::Vector2f offset) {
    for (auto& c : children) {
        c->Render(window, offset + c->position);
    }
}

RenderChunkText::RenderChunkText(std::string text_, sf::Vector2f offset)
    : RenderChunk(offset)
{
    text = makeText(text_);
}

sf::Vector2f RenderChunkText::ComputeSize(){
    auto b = text.getLocalBounds();
    return {b.width+b.left, b.height+b.top};
}

void RenderChunkText::Render(sf::RenderWindow& window, sf::Vector2f offset) {
    text.setPosition(offset);
    window.draw(text);
}

/////////////// UTILS

sf::Text makeText(std::string t)
{
    sf::Text text;
    text.setFont(font); // font is a sf::Font
    text.setString(t);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(FONT_SIZE); // in pixels, not points!
    return text;
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& v) {
    os << v.x << "," << v.y;
}

int stringOccurrences(const std::string& text, std::regex& expression) {
    std::ptrdiff_t const match_count(std::distance(
        std::sregex_iterator(text.begin(), text.end(), expression),
        std::sregex_iterator()));
    return match_count;
}