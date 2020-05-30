#ifndef LEXER_COMPILER_H
#define LEXER_COMPILER_H

#include <fstream>
#include <iostream>
#include "Node.h"
#include "Parser.h"

using namespace std;

class Compiler {
    string SECTION_DATA = "SECTION .data\n";
    string SECTION_TEXT = "SECTION .text\n";
//    vector<string> GLOBAL_MARKS;
    vector<string> expressions;
    map<string, pair<lexemtype, string>> VARIABLES;
//    map<string, int> FUNCTIONS;
    set<string> FUNCTIONS;
    string TEXT = "";
    string ASSEMBLY_FILE_NAME = "object.txt";

    int currentExp = 0;
    int IF_COUNT = 0;
    int WHILE_COUNT = 0;
    int ELSE_COUNT = 0;
    int FALSE_COUNT = 0;

    void treeTraversal(Node &node, string &str);

    void print();

public:
    Compiler() = default;

    explicit Compiler(Parser &parser) {
        VARIABLES = parser.VARIABLES;
        FUNCTIONS = parser.FUNCTIONS;
    }

    void compile(Node &node);

};


#endif //LEXER_COMPILER_H
