#include <fstream>
#include <set>
#include <iomanip>
#include "Parser.h"
#include "Compiler.h"
#include "nodekind.h"
#include <random>

ostream &operator<<(ostream &out, const lexemtype &t) {
    switch (t) {
        case OBRA:
            return (out << "Open brace");
        case CBRA:
            return (out << "Close brace");
        case OPAR:
            return (out << "Open parenthesis");
        case CPAR:
            return (out << "Close parenthesis");
        case SEMICOLON:
            return (out << "Semicolon operator");
        case INTKW:
            return (out << "Int keyword");
        case RETURNKW:
            return (out << "Return keyword");
        case IFKW:
            return (out << "If keyword");
        case ELSEKW:
            return (out << "Else keyword");
        case IDENTIFIER:
            return (out << "Identifier");
        case INTLITERAL:
            return (out << "Int literal");
        case null:
            break;
        case FORKW:
            return (out << "For keyword");
        case WHILEKW:
            return (out << "While keyword");
        case EQUAL:
            return (out << "Equal sign");
        case LESS:
            return (out << "Less sign");
        case MORE:
            return (out << "More sign");
        case PLUS:
            return (out << "Plus sign");
        case MINUS:
            return (out << "Minus sign");
        case MULTIPLICATION:
            return (out << "Multiplication sign");
        case DIVISION:
            return (out << "Division sign");
        case LOGIC_NEG:
            return (out << "Logic negation sign");
//        case DOUBLEKW:
//            break;
//        case CHARKW:
//            break;
//        case FLOATKW:
//            break;
        case COMMA:
            return (out << "Comma sign");
    }
    return (out);
}

ostream &operator<<(ostream &out, const nodekind &n) {
    switch (n) {
        case PROG:
            return (out << "Program");
        case FUNC:
            return (out << "Function");
        case RETURN:
            return (out << "Return");
        case FOR:
            return (out << "For");
        case IF:
            return (out << "If");
        case WHILE:
            return (out << "While");
        case CONST:
            return (out << "Const");
        case EXPRESSION:
            return (out << "Expression");
        case VAR:
            return (out << "Var");
        case SET:
            return (out << "Set");
        case COMPL:
            return (out << "Comp op: \"<\"");
        case COMPR:
            return (out << "Comp op: \">\"");
        case ADD:
            return (out << "Add");
        case SUB:
            return (out << "Sub");
        case EMPTY:
            return (out << "Empty");
        case SEQ:
            return (out << "Statement");
        case MUL:
            return (out << "Multiplication");
        case DIV:
            return (out << "Division");
        case UN_OP_EXPR_N:
            return (out << "Negation: op: \"-\"");
        case UN_OP_EXPR_L_N:
            return (out << "Negation: op: \"!\"");
        case ELSE:
            return (out << "Else");
        case INT_DECL:
            return (out << "Int declaration");
        case DOUBLE_DECL:
            return (out << "Double declaration");
        case FLOAT_DECL:
            return (out << "Float declaration");
        case CHAR_DECL:
            return (out << "Char declaration");
        case ASSIGNMENT:
            return (out << "Assignment");
        case FUNC_CALL:
            return (out << "Call");
    }
}

void printTree(Node node, int space) {
    for (int i = 0; i < space; i++) {
        cout << " ";
    }
    cout << node.getKind() << " " << node.value << endl;
    for (const auto &n : node.subnodes) {
        printTree(n, space + 5);
    }
}

int rnd() {
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(0, 200);
    return distribution(generator);
}

void lab2_3(Parser &parser) {
    map<string, lexemtype> table = parser.getLexer().table;
    cout << "            Lexemes table" << endl;
    int i = 1;
    cout << " Num |  Lexem  |      Definition      |" << endl;
    cout << "-----+---------+----------------------+" << endl;
    for (const auto &str : table) {
        cout << setw(3) << i << ". | " << setw(7) << str.first << " | " << setw(20)
             << str.second << " | " << endl;
        ++i;
    }
}

int hashFunction(string word) {
    int result = 0;
    for (int i = 0; i < word.size(); ++i) {
        result += word[i] * pow(4, i);
    }
    return result;
}

void lab4(Parser &parser) {
    map<string, lexemtype> table = parser.getLexer().table;
    map<int, pair<string, lexemtype>> hashTable;
    pair<string, lexemtype> pair;
    for (const auto &str : table) {
        int key = hashFunction(str.first);
        pair = str;
        hashTable.emplace(key, pair);
    }
    cout << "                Hash lexemes table" << endl;
    cout << " Num |    HASHKEY   |  Lexem  |      Definition      |" << endl;
    cout << "-----+--------------+---------+----------------------+" << endl;
    int i = 1;
    for (const auto &str : hashTable) {
        cout << setw(3) << i << ". | " << setw(10) << str.first << " . | " << setw(7)
             << str.second.first << " | " << setw(20) << str.second.second << " | " << endl;
        ++i;
    }
}

// в файле file.txt убрать ";" после func() или "(" после main дабы продемонстрировать, что ошибки
// обрабатываются
void lab5() {}

void lab6(Node &ast) {
    printTree(ast, 0);
}

// ассемблерный код записывается в файл object.txt
void lab7(Parser &parser, Node &ast) {
    Compiler compiler = Compiler(parser);
    compiler.compile(ast);
}

int main(int argc, char *argv[]) {
    Parser parser = Parser();
    Node ast = parser.parse();
    lab2_3(parser); // done
//    lab4(parser); // done
//    lab5(); // done
//    lab6(ast); // done
//    lab7(parser, ast); // done
    return 0;

}