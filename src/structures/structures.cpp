#include "structures.hpp"

std::string TerminalNode::ToString() {
    return this->value;
}

std::string NonTerminalNode::ToString() {
    std::string ret;
    for (auto& child : this->children) {
        ret += child->ToString() + " ";
    }
    ret.pop_back();
    return ret;
}

std::string FlatListNode::ToString() {
    std::string ret;
    for (auto& child : this->children) {
        ret += child->ToString() + separator->GetValue();
    }
    ret.pop_back();
    return ret;
}