#ifndef LEXER_NODEKIND_H
#define LEXER_NODEKIND_H

enum nodekind {
    PROG, FUNC, RETURN, FOR, IF, WHILE, CONST, EXPRESSION, VAR, SET, COMPL, COMPR, ADD, SUB, MUL,
    DIV, EMPTY, SEQ, UN_OP_EXPR_N, UN_OP_EXPR_L_N, ELSE, INT_DECL, DOUBLE_DECL, FLOAT_DECL,
    CHAR_DECL, ASSIGNMENT, FUNC_CALL
};

#endif //LEXER_NODEKIND_H