#include "Parser.h"
#include <sstream>
#include <set>

Node Parser::parse() { // fixme pointer to node ???
    nextLexem();
    Node progNode = Node(PROG);
    while (!fin.eof()) {
        progNode.addSubnode(function());
    }
    fin.close();
    return progNode;
}


// <function> = int <id> () <statement>
Node Parser::function() {
    std::string funcName;
    if (!isDataType()) {
        cout << "Unexpected unqualified-id \"" << getLexemValue() << "\"" << endl;
        exit(0);
    }
    nextLexem();
    if (getLexemType() != IDENTIFIER) {
        cout << "Unexpected unqualified-id \"" << getLexemValue() << "\"" << endl;
        exit(0);
    }
    funcName = getLexemValue();
    if (doesFunctionExist(funcName)) {
        cout << "Function with that name already exists" << endl;
        exit(0);
    }
    FUNCTIONS.insert(funcName);
//    for (auto i : FUNCTIONS) {
//        cout << i << endl;
//    }
//    FUNCTIONS[funcName] = 1;
    nextLexem();
    if (getLexemType() != OPAR) {
        cout << "\"(\" expected" << endl;
        exit(0);
    }
    nextLexem();
    if (getLexemType() != CPAR) {
        cout << "\")\" expected" << endl;
        exit(0);
    }
    nextLexem();
    Node funcNode = Node(FUNC, funcName);
    funcNode.addSubnode(statement());
    if (!didFuncReturn) {
        cout << "Function \"" << funcName << "\" must return statement!" << endl;
        exit(0);
    }
    didFuncReturn = false;
    nextLexem();
    return funcNode;
}

// <statement> = { "while" <paren_exp> <statement> |
//               "if" <paren_exp> <statement> |
//               "if" <paren_exp> <statement> "else" <statement> |
//               "return" <statement> |
//               "{" <statement> "}" |
//               <var_declaration> ;
//               <expression> ";" |
//               ";" }
Node Parser::statement() {
    Node stNode;
    if (getLexemType() == WHILEKW) { // while statement
        stNode = Node(WHILE);
        nextLexem();
        stNode.addSubnode(paren_expression());
        stNode.addSubnode(statement());
    } else if (getLexemType() == IFKW) { // if statement
        stNode = Node(IF);
        nextLexem();
        stNode.addSubnode(paren_expression());
        stNode.addSubnode(statement());
        nextLexem();
        stNode._if = true;
        if (getLexemType() == ELSEKW) {
            stNode._else = true;
            Node elseNode = Node(ELSE);
            nextLexem();
            elseNode.addSubnode(statement());
            stNode.addSubnode(elseNode);
        }
    } else if (getLexemType() == RETURNKW) { // return statement
        stNode = Node(RETURN);
        nextLexem();
        stNode.addSubnode(statement());
        didFuncReturn = true;
    } else if (getLexemType() == OBRA) {
        stNode = Node(SEQ);
        nextLexem();
        while (getLexemType() != CBRA) {
            Node _stNode = Node(statement());
            if (!_stNode._if || _stNode._else) {
                nextLexem();
            }
            if (getLexemType() == null) {
                cout << "\"}\" expected" << endl;
                exit(0);
            }
            stNode.addSubnode(_stNode);
        }
    } else if (getLexemType() == CBRA) {
        stNode = Node(EMPTY);
    } else if (isDataType()) {
        stNode = Node(var_declaration());
    } else { // todo statement SEQ must has more than one expressions
        stNode = Node(EXPRESSION);
        stNode.addSubnode(expression());
        if (getLexemType() != SEMICOLON) {
            cout << "\";\" expected" << endl;
            exit(0);
        }
    }
    return stNode;
}

void Parser::addDeclVar(const lexemtype &type, Node &assignNode, Node &declNode) {
    nextLexem();
    if (getLexemType() != IDENTIFIER) {
        cout << "Unexpected unqualified-id \"" << getLexemValue() << "\"" << endl;
        exit(0);
    }
    if (doesVarExist()) {
        cout << "Error. Variable \"" << getLexemValue() << "\" has already been declared" << endl;
        exit(0);
    }
    pair<lexemtype, string> pair = { type, NO_VALUE};
    string variable = getLexemValue();
    VARIABLES.emplace(variable, pair);
    declNode.addSubnode(Node(VAR, getLexemValue()));
    nextLexem();
    if (getLexemType() == EQUAL) {
        assignNode = Node(ASSIGNMENT);
        nextLexem();
        assignNode.addSubnode(expression());
        if (assignNode.subnodes[0].getKind() == CONST) {
            pair.second = assignNode.subnodes[0].value;
            VARIABLES[variable] = pair;
        }
        declNode.addSubnode(assignNode);
    }
}

// <var_declaration> = { "DATA_TYPE" <id> [ "=" <expression> ] [ "," <id> [ "=" <expression> ] ] ; }
Node Parser::var_declaration() {
    nodekind kind = EMPTY;
    lexemtype type = getLexemType();
    switch (type) {
        case INTKW:
            kind = INT_DECL;
            break;
        case DOUBLEKW:
            kind = DOUBLE_DECL;
            break;
        case FLOATKW:
            kind = FLOAT_DECL;
            break;
        case CHARKW:
            kind = CHAR_DECL;
            break;
        default:
            cout << "Unknown type" << endl;
            exit(0);
    }
    Node assignNode;
    Node declNode = Node(kind);
    addDeclVar(type, assignNode, declNode);
    while (getLexemType() != SEMICOLON) {
        if (getLexemType() == COMMA) {
            addDeclVar(type, assignNode, declNode);
        }
    }
    // todo calculate expression and add it variables map as value
    if (getLexemType() != SEMICOLON) {
        cout << "\";\" expected" << endl;
        exit(0);
    }
    return declNode;
}

// <paren_exp> = { "(" <expression> ")" }
Node Parser::paren_expression() {
    Node parenExpNode;
    if (getLexemType() != OPAR) {
        cout << "\"(\" expected" << endl;
        exit(0);
    }
    nextLexem();
    parenExpNode = expression();
    if (getLexemType() != CPAR) {
        cout << "\")\" expected" << endl;
        exit(0);
    }
    nextLexem();
    return parenExpNode;
}

// <expression> = { <test> | <id> = <expression> | <function_call> }
Node Parser::expression() {
    if (getLexemType() != IDENTIFIER) {
        return test();
    }
    Node expNode = test();
    if (expNode.getKind() == FUNC_CALL) {
        nextLexem();
        if (getLexemType() != CPAR) {
            cout << "\")\" expected" << endl;
            exit(0);
        }
        nextLexem();
    }
    if (getLexemType() == EQUAL) {
        if (expNode.getKind() == UN_OP_EXPR_L_N || expNode.getKind() == UN_OP_EXPR_N) {
            cout << "Error. Expression is not assignable" << endl;
            exit(0);
        }
        if (expNode.getKind() == VAR) {
            nextLexem();
            Node varNode = Node(expNode);
            expNode = Node(SET);
            expNode.addSubnode(varNode);
            expNode.addSubnode(expression());
        }
    }
    if (getLexemType() == INTLITERAL) {
        expNode = Node(CONST, getLexemValue());
        nextLexem();
    }
    return expNode;
}

// <test> = { <sum> | <sum> < <sum> | <sum> > <sum> }
Node Parser::test() { // todo make "compare" with operator
    Node testNode = sum();
    if (getLexemType() == LESS) {
        nextLexem();
        Node thisNode = Node(testNode);
        testNode = Node(COMPL);
        testNode.addSubnode(thisNode);
        testNode.addSubnode(sum());
    } else if (getLexemType() == MORE) {
        nextLexem();
        Node thisNode = Node(testNode);
        testNode = Node(COMPR);
        testNode.addSubnode(thisNode);
        testNode.addSubnode(sum());
    }
    return testNode;
}

// <sum> = { <term> | <sum> "+" <term> | <sum> "-" <term> | <sum> "*" <term> | <sum> "/" <term>
//         |  }
Node Parser::sum() {
    Node termNode = term(); // sumNode = var
    nodekind kind;
    while (getLexemType() == PLUS || getLexemType() == MINUS || getLexemType() == MULTIPLICATION
           || getLexemType() == DIVISION) {
        switch (getLexemType()) {
            case PLUS:
                kind = ADD;
                break;
            case MINUS:
                kind = SUB;
                break;
            case MULTIPLICATION:
                kind = MUL;
                break;
            case DIVISION:
                kind = DIV;
                break;
        }
        Node tNode = Node(termNode);
        termNode = Node(kind, getLexemValue());
        nextLexem();
        termNode.addSubnode(tNode);
        termNode.addSubnode(term());
    }
    return termNode;
}

// <term> = { <id> | <int> | <paren_exp> | "-" ... | "!" ... }
Node Parser::term() {
    Node termNode;
    if (getLexemType() == LOGIC_NEG || getLexemType() == MINUS) {
        nodekind kind = EMPTY;
        termNode = Node(getLexemType() == LOGIC_NEG ? UN_OP_EXPR_L_N : UN_OP_EXPR_N);
        nextLexem();
        termNode.addSubnode(term());
        return termNode;
    }
    if (getLexemType() == IDENTIFIER) {
        string idValue = getLexemValue();
        nextLexem();
        if (getLexemType() == OPAR) {
            if (!doesFunctionExist(idValue)) {
                cout << "Function \"" << idValue << "\" was not declared" << endl;
                exit(0);
            }
            termNode = Node(FUNC_CALL, idValue);
            return termNode;
        }
        if (!doesVarExist(idValue)) {
            cout << "Variable \"" << idValue << "\" was not declared" << endl;
            exit(0);
        }
        termNode = Node(VAR, idValue);
        return termNode;
    }
    if (getLexemType() == INTLITERAL) {
        termNode = Node(CONST, getLexemValue());
        nextLexem();
        return termNode;
    }
    return paren_expression();
}



























