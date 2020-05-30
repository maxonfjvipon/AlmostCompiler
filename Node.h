#ifndef LEXER_NODE_H
#define LEXER_NODE_H

#include "nodekind.h"
#include <iostream>
#include <vector>

using namespace std;

class Node {
    nodekind kind;

public:
    bool _if = false;
    bool _else = false;
    string value;
    vector<Node> subnodes;
    Node() = default;

    Node (nodekind kind, const string &value = "");

    Node (const Node &node);

    void addSubnode(Node subnode) {
        subnodes.push_back(subnode);
    }

    nodekind getKind() {
        return kind;
    }

};


#endif //LEXER_NODE_H
