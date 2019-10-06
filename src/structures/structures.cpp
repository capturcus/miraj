#include "structures.hpp"

sf::Text makeText(std::string t);

/////////////// DISPLAY NODES

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

std::unique_ptr<RenderChunk> TerminalNode::Render(sf::Vector2f offset)
{
    // auto ret = std::make_unique<RenderChunk>();
    // ret->texts.push_back(makeText(this->value));
    // return std::move(ret);
    return std::unique_ptr<RenderChunkRectangle>(new RenderChunkRectangle(sf::RectangleShape({2,2}), sf::Color::Red, {50,50}));
}

std::unique_ptr<RenderChunk> NonTerminalNode::Render(sf::Vector2f offset)
{
    // // find the production that the current node is in
    // auto productions = this->nonTerminal->GetProductions();
    // Production *currentProduction = nullptr;
    // for (auto &p : productions)
    // {
    //     if (p.number == this->prodNumber)
    //     {
    //         currentProduction = &p;
    //         break;
    //     }
    // }
    // if (currentProduction == nullptr)
    // {
    //     std::cout << "ERROR currentProduction " << this->prodNumber << " not found\n";
    //     exit(1);
    // }

    // // auto spaceBounds = makeText(" ").getLocalBounds();

    // RenderChunk *ret = new RenderChunk();
    // int currentXOffset = 0;
    // int currentYOffset = 0;
    // // int rowBound = 0;

    // size_t displayCharCount = 0;
    // size_t childCount = 0;

    // // if empty just display first child
    // if (currentProduction->displayFormat == "")
    // {
    //     currentProduction->displayFormat = "@";
    // }

    // std::cout << "DISPLAY FORMAT " << currentProduction->displayFormat << "\n";

    // while (displayCharCount < currentProduction->displayFormat.size())
    // {
    //     auto ch = currentProduction->displayFormat[displayCharCount];
    //     if (ch == '@')
    //     {
    //         displayCharCount++;
    //         std::cout << "DISCOVERED NUT " << childCount << " "
    //                   << "\n";
    //         auto &child = this->children[childCount];
    //         auto chunk = child->Render(offset + sf::Vector2f(currentXOffset, 0));
    //         chunk->position.x = currentXOffset+offset.x;
    //         chunk->position.y = currentYOffset+offset.y;
    //         currentXOffset += chunk->ComputeSize().x;
    //         ret->children.push_back(std::move(chunk));
    //         childCount++;
    //     }
    //     else if (ch == ' ')
    //     {
    //         std::cout << "SPACE\n";
    //     }
    //     else if (ch == '\t')
    //     {
    //         std::cout << "TAB\n";
    //     }
    //     else if (ch == '\n')
    //     {
    //         std::cout << "NEWLINE\n";
    //     }
    //     else
    //     {
    //         std::cout << "wrong char in display format: " << ch << "\n";
    //     }
    //     displayCharCount++;
    // }

    // return std::unique_ptr<RenderChunk>(ret);
    return std::unique_ptr<RenderChunkRectangle>(new RenderChunkRectangle(sf::RectangleShape({2,2}), sf::Color::Red, {50,50}));
}

std::unique_ptr<RenderChunk> FlatListNode::Render(sf::Vector2f offset)
{
    // auto ret = std::make_unique<RenderChunk>();
    // TODO if the separator is a newline we have to treat this differently

    // auto spaceBounds = makeText(" ").getLocalBounds();

    // int currentXOffset = 0;
    // int currentYOffset = 0;
    // for (size_t i = 0; i < children.size(); i++)
    // {
    //     auto& child = children[i];
    //     auto chunk = child->Render(offset + sf::Vector2f(currentXOffset, 0));
    //     chunk->position.x = currentXOffset+offset.x;
    //     chunk->position.y = currentYOffset+offset.y;
    //     currentXOffset += chunk->ComputeSize().x;
    //     ret->children.push_back(std::move(chunk));
    //     // also push the separator if not at the end
    //     if (i != children.size()-1) {
    //         currentXOffset += spaceBounds.width;
    //         auto text = makeText(this->flatList->separator->GetValue());
    //         text.setPosition(offset + sf::Vector2f(currentXOffset, 0));
    //         auto textWidth = text.getLocalBounds().width;
    //         ret->texts.push_back(text);
    //         currentXOffset += textWidth + spaceBounds.width;
    //     }
    // }

    // return std::move(ret);
    return std::unique_ptr<RenderChunkRectangle>(new RenderChunkRectangle(sf::RectangleShape({2,2}), sf::Color::Red, {50,50}));
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
    text.setCharacterSize(20); // in pixels, not points!
    return text;
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& v) {
    os << v.x << "," << v.y;
}
