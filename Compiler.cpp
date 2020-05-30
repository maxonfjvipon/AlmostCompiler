#include "Compiler.h"
#include "nodekind.h"

void Compiler::print() {
    ifstream fin(ASSEMBLY_FILE_NAME);
    string s;
    while (getline(fin, s)) {
        cout << s << endl;
    }
    fin.close();
}

void Compiler::compile(Node &node) {
    treeTraversal(node, TEXT);
    ofstream fout(ASSEMBLY_FILE_NAME);
    fout << SECTION_DATA;
    for (const auto &var : VARIABLES) {
        fout << var.first << ": resd 1" << endl;
    }
    fout << "\n";
    fout << SECTION_TEXT;
    for (auto &func : FUNCTIONS) {
        fout << "global " << func << endl;
    }
    fout << "\n";
    fout << TEXT;
    for (auto exp : expressions) {
        fout << exp << endl;
    }
    fout.close();
//    print();
}

void Compiler::treeTraversal(Node &node, string &str) {
    switch (node.getKind()) {
        case PROG:
            for (auto n : node.subnodes) {
                treeTraversal(n, TEXT);
            }
            break;
        case FUNC:
            TEXT += (node.value + ":\n");
            treeTraversal(node.subnodes[0], TEXT);
//            TEXT += "ret\n";
            break;
        case SEQ:
            for (auto n : node.subnodes) {
                treeTraversal(n, TEXT);
            }
            break;
        case RETURN:
            treeTraversal(node.subnodes[0], TEXT);
            TEXT += ("ret\n\n");
            break;
        case INT_DECL:
            if (node.subnodes.size() > 1) {
                int pos = 1;
                while (pos < node.subnodes.size()) {
                    if (node.subnodes[pos].getKind() == ASSIGNMENT) {
                        treeTraversal(node.subnodes[pos], node.subnodes[pos - 1].value);
                    }
                    ++pos;
                }
            }
            break;
        case FUNC_CALL: {
            str += "call " + node.value + "\n";
            break;
        }
//        case DOUBLE_DECL:
//            break;
//        case FLOAT_DECL:
//            break;
//        case CHAR_DECL:
//            break;
        case FOR:
            break;
        case IF: {
            int IF_C = IF_COUNT;
            ++IF_COUNT;
            TEXT += "\nIF" + to_string(IF_C) + ":\ncall exp"
                    + to_string(currentExp) + "\n";
            treeTraversal(node.subnodes[0], TEXT);
            TEXT += "cmp eax, 1\njne .ifalse" + to_string(IF_C) + "\n";
            treeTraversal(node.subnodes[1], TEXT);
            TEXT += ".ifalse" + to_string(IF_C) + ":\n";
            if (node.subnodes.size() == 3) {
                treeTraversal(node.subnodes[2], TEXT);
            }
            break;
        }
        case ELSE:
            TEXT += "\nELSE" + to_string(ELSE_COUNT) + ":\n";
            ++ELSE_COUNT;
            treeTraversal(node.subnodes[0], TEXT);
            break;
        case WHILE: {
            int W_C = WHILE_COUNT;
            ++WHILE_COUNT;
            TEXT += "\nWHILE" + to_string(W_C) + ":\n.CMP" + to_string(W_C) + "\ncall exp"
                    + to_string(currentExp) + "\n";
            treeTraversal(node.subnodes[0], TEXT);
            TEXT += "cmp eax, 1\njne .wfalse" + to_string(W_C) + "\n";
            treeTraversal(node.subnodes[1], TEXT);
            TEXT += "jmp .CMP" + to_string(W_C) + "\n.wfalse" + to_string(W_C) + ":\n";
            break;
        }
        case CONST:
            str += "mov eax, " + node.value + "\n";
            break;
        case EXPRESSION: {
            TEXT += "call exp" + to_string(currentExp) + "\n";
            string exp = "exp" + to_string(currentExp) + ":\n";
            currentExp++;
            treeTraversal(node.subnodes[0], exp);
            exp += "ret\n";
            expressions.emplace_back(exp);
            break;
        }
        case VAR:
            str += "mov eax, " + node.value + "\n";
            break;
        case SET:
            treeTraversal(node.subnodes[1], str);
            str += ("mov " + node.subnodes[0].value + ", eax\n");
            break;
        case UN_OP_EXPR_N:
            treeTraversal(node.subnodes[0], str);
            str += "neg eax\n";
            break;
        case UN_OP_EXPR_L_N:
            treeTraversal(node.subnodes[0], str);
            str += "cmp 0, eax\nmov eax, 0\nsete al\n";
            break;
        case ASSIGNMENT: {
            TEXT += "call exp" + to_string(currentExp) + "\n";
            string exp = "exp" + to_string(currentExp) + ":\n";
            currentExp++;
            treeTraversal(node.subnodes[0], exp);
            exp += "ret\n";
            TEXT += "mov " + str + ", eax\n";
            expressions.emplace_back(exp);
            break;
        }
        case COMPL: {
            string exp = "exp" + to_string(currentExp) + ":\n";
            ++currentExp;
            treeTraversal(node.subnodes[0], exp);
            exp += "push eax\n";
            treeTraversal(node.subnodes[1], exp);
            exp += "pop ecx\ncmp ecx, eax\njae .false\nmov eax, 1\nret\n.false:\nmov eax,"
                   "0\nret\n";
            expressions.emplace_back(exp);
            break;
        }
        case COMPR: {
            string exp = "exp" + to_string(currentExp) + ":\n";
            ++currentExp;
            treeTraversal(node.subnodes[0], exp);
            exp += "push eax\n";
            treeTraversal(node.subnodes[1], exp);
            exp += "pop ecx\ncmp ecx, eax\njbe .false\nmov eax, 1\nret\n.false:\nmov eax,"
                   "0\nret\n";
            expressions.emplace_back(exp);
            break;
        }
        case ADD:
            treeTraversal(node.subnodes[0], str);
            str += "push eax\n";
            treeTraversal(node.subnodes[1], str);
            str += "pop ecx\nadd eax, ecx\n";
            break;
        case SUB:
            treeTraversal(node.subnodes[0], str);
            str += "push eax\n";
            treeTraversal(node.subnodes[1], str);
            str += "pop ecx\nsub ecx, eax\nmov eax, ecx\n";
            break;
        case MUL:
            treeTraversal(node.subnodes[0], str);
            str += "push eax\n";
            treeTraversal(node.subnodes[1], str);
            str += "pop ecx\nimul ecx\n";
            break;
        case DIV:
            treeTraversal(node.subnodes[0], str);
            str += "push eax\n";
            treeTraversal(node.subnodes[1], str);
            str += "mov ecx, eax\npop eax\nidiv ecx\n";
            break;
        case EMPTY:
            break;
    }
}
