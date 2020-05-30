#include "Node.h"

Node::Node(nodekind kind, const string &value) {
    this->kind = kind;
    this->value = value;
}

Node::Node(const Node &node) {
    kind = node.kind;
    value = node.value;
    for (auto &item : node.subnodes) {
        subnodes.push_back(item);
    }
}