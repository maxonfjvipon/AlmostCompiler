#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include "lexemtype.h"

using namespace std;

struct Lexem {
    lexemtype type;
    string value;

    Lexem() = default;
};

class Lexer {
    map<char , lexemtype> SYMBOLS = {
            {'{', OBRA},
            {'}', CBRA},
            {'(', OPAR},
            {')', CPAR},
            {';', SEMICOLON},
            {'=', EQUAL},
            {'<', LESS},
            {'>', MORE},
            {'+', PLUS},
            {'-', MINUS},
            {'*', MULTIPLICATION},
            {'/', DIVISION},
            {'!', LOGIC_NEG},
            {',', COMMA}
    };
    map<string, lexemtype> KEYWORDS = {
            {"return", RETURNKW},
            {"if", IFKW},
            {"else", ELSEKW},
            {"while", WHILEKW},
            {"for", FORKW}
    };
    map<string, lexemtype> DATA_TYPES {
            {"int", INTKW},
            {"double", DOUBLEKW},
            {"char", CHARKW},
            {"float", FLOATKW}
    };

    char ch = ' ';

    bool inSymbols(const char &ch) {
        return SYMBOLS.count(ch) != 0;
    }

    bool inDataTypes(const string &str) {
        return DATA_TYPES.count(str) != 0;
    }

    bool inKeywords(const string &str) {
        return KEYWORDS.count(str) != 0;
    }

    bool isDigit(const char &ch) {
        return (ch >= '0' && ch <= '9');
    }

    bool isAlfa(const char &ch) {
        return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
    }

    Lexem lexem = Lexem();

public:

    map<string, lexemtype> table;

    void print() {
        int i = 1;
        for (const auto& sym : table) {
            cout << i << ". " << sym.first << " " << sym.second << endl;
            ++i;
        }
    }

    bool isDataType() {
        return DATA_TYPES.count(getLexemValue()) != 0;
    }

    lexemtype getLexemType() {
        return lexem.type;
    }

    string getLexemValue() {
        return lexem.value;
    }

    Lexer() = default;

    void nextLexem(ifstream &fin);

};


#endif //LEXER_LEXER_H
