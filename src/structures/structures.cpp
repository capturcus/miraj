#include "structures.hpp"

std::string TerminalNode::ToString() {
    return this->value;
}

std::string NonTerminalNode::ToString() {
    std::string ret;
    for (auto& child : this->children) {
        ret += child->ToString() + " ";
    }
    if (ret.length() > 0) {
        ret.pop_back();
    }
    return ret;
}

std::string FlatListNode::ToString() {
    std::string ret;
    for (auto& child : this->children) {
        ret += child->ToString() + flatList->separator->GetValue();
    }
    if (ret.length() > 0) {
        ret.pop_back(); // fixme what if the separator is longer than 1 char
    }
    return ret;
}