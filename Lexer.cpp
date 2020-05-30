#include "Lexer.h"

void Lexer::nextLexem(ifstream &fin) {
    string value;
    lexem.type = null;
    if (fin.eof()) {
        lexem.type = null;
        return;
    }
    while(lexem.type == null) {
        if (ch == ' ' || ch == '\n') {
            ch = fin.get();
            continue;
        }
        if (inSymbols(ch)) {
            lexem.type = SYMBOLS[ch];
            lexem.value = string(1,  ch);
            table.emplace(lexem.value, lexem.type);
            ch = fin.get();
            continue;
        }
        if (isDigit(ch)) {
            value = "";
            while (isDigit(ch)) {
                value += ch;
                ch = fin.get();
            }
            lexem.type = INTLITERAL;
            lexem.value = value;
            table.emplace(lexem.value, lexem.type);
            continue;
        }
        if (isAlfa(ch)) {
            value = "";
            while (isAlfa(ch) || isDigit(ch)) {
                value += ch;
                ch = fin.get();
            }
            if (inKeywords(value)) {
                lexem.type = KEYWORDS[value];
                lexem.value = value;
                table.emplace(lexem.value, lexem.type);
                continue;
            }
            if (inDataTypes(value)) {
                lexem.type = DATA_TYPES[value];
                lexem.value = value;
                table.emplace(lexem.value, lexem.type);
                continue;
            }
            lexem.type = IDENTIFIER;
            lexem.value = value;
            table.emplace(lexem.value, lexem.type);
        }
    }
}

