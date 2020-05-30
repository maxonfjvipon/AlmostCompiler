#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include "Lexer.h"
#include "Node.h"

#define NO_VALUE "no_value"
#define DEFAULT_VALUE "default_value"

using namespace std;

class Parser {
    string executeFileName = "file.txt";

    ifstream fin;
    Lexer lexer;

    bool didFuncReturn = false;

    Node function();

    Node statement();

    Node expression();

    Node var_declaration();

    Node paren_expression();

    Node test();

    Node sum();

    Node term();

    void nextLexem() {
        lexer.nextLexem(fin);
    }

    string getLexemValue() {
        return lexer.getLexemValue();
    }

    inline bool isDataType() {
        return lexer.isDataType();
    }

    lexemtype getLexemType() {
        return lexer.getLexemType();
    }

    void addDeclVar(const lexemtype &type, Node &assignNode, Node &declNode);

    inline bool doesVarExist(string value = DEFAULT_VALUE) {
        if (value == DEFAULT_VALUE) {
            value = getLexemValue();
        }
        return VARIABLES.count(value);
    }

    inline bool doesFunctionExist(string funcName = DEFAULT_VALUE) {
        if (funcName == DEFAULT_VALUE) {
            funcName = getLexemValue();
        }
        return FUNCTIONS.count(funcName);
    }

public:
    Lexer getLexer() {
        return lexer;
    }

    map<string, pair<lexemtype, string>> VARIABLES {};
//    map<string, int> FUNCTIONS {};
    std::set<string> FUNCTIONS;

    Parser() {
        fin.open(executeFileName);
        lexer = Lexer();
    };


    Node parse();

};


#endif //LEXER_PARSER_H
