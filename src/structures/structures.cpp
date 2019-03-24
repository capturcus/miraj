#include "structures.hpp"

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

void ColoredRectangle::Render(sf::RenderWindow &window) {

}

void RenderChunk::Render(sf::RenderWindow &window)
{
    for (auto& t : texts) {
        window.draw(t);
    }
    for (auto& r : rects) {
        r.Render(window);
    }
    for(auto& c : children) {
        c->Render(window);
    }
}

sf::Vector2f ensureCover(sf::Vector2f coveringRect, sf::FloatRect coveredRect) {
    sf::Vector2f ret;
    ret.x = std::max(coveredRect.width+coveredRect.left, coveringRect.x);
    ret.y = std::max(coveredRect.height+coveredRect.top, coveringRect.y);
    // for now ignore top and left
    return ret;
}

sf::Vector2f RenderChunk::ComputeSize()
{
    sf::Vector2f ret;
    for (auto& t : texts) {
        auto bounds = t.getLocalBounds();
        ret = ensureCover(ret, bounds);
    }
    return ret;
}

sf::Text makeText(std::string t)
{
    sf::Text text;
    text.setFont(font); // font is a sf::Font
    text.setString(t);
    text.setColor(sf::Color::White);
    text.setCharacterSize(24); // in pixels, not points!
    return text;
}

std::unique_ptr<RenderChunk> TerminalNode::Render(sf::Vector2f offset)
{
    auto ret = std::make_unique<RenderChunk>();
    ret->texts.push_back(makeText(this->value));
    return std::move(ret);
}

std::unique_ptr<RenderChunk> NonTerminalNode::Render(sf::Vector2f offset)
{
    std::cout << "NTN RENDER\n";

    // find the production that the current node is in
    auto productions = this->nonTerminal->GetProductions();
    Production *currentProduction = nullptr;
    for (auto &p : productions)
    {
        if (p.number == this->prodNumber)
        {
            currentProduction = &p;
            break;
        }
    }
    if (currentProduction == nullptr)
    {
        std::cout << "ERROR currentProduction not found\n";
        exit(1);
    }

    auto spaceBounds = makeText(" ").getLocalBounds();

    RenderChunk *ret = new RenderChunk();
    int currentXOffset = 0;
    int currentYOffset = 0;
    int rowBound = 0;

    size_t displayCharCount = 0;
    size_t childCount = 0;

    while (displayCharCount < currentProduction->displayFormat.size())
    {
        auto ch = currentProduction->displayFormat[displayCharCount];
        if (ch == '@')
        {
            displayCharCount++;
            std::cout << "DISCOVERED NUT " << childCount << " "
                      << "\n";
            auto &child = this->children[childCount];
            auto chunk = child->Render(offset + sf::Vector2f(currentXOffset, 0));
            chunk->position.x = currentXOffset;
            chunk->position.y = currentYOffset;
            currentXOffset += chunk->ComputeSize().x;
            ret->children.push_back(std::move(chunk));
        }
        else if (ch == ' ')
        {
        }
        else if (ch == '\t')
        {
        }
        else if (ch == '\n')
        {
        }
        else
        {
            std::cout << "wrong char in display format: " << ch << "\n";
        }
        displayCharCount++;
    }

    return std::unique_ptr<RenderChunk>(ret);
}

std::unique_ptr<RenderChunk> FlatListNode::Render(sf::Vector2f offset)
{
    return nullptr;
}