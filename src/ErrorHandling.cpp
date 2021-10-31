//
// Created by tjan2 on 15/10/2021.
//

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "ErrorHandling.h"
using namespace std;

ErrorHandling::ErrorHandling() {

}

void ErrorHandling::pushError(int line, char err) {
    errors.emplace_back(line, err);
}

void ErrorHandling::formatStringError(const Symbol& sym) {
    int line = sym.row;
    string str = sym.val;
    if (str[0] != '\"' || str[str.size() - 1] != '\"') {
        pushError(line, 'a');
    } else {
        for (int i = 1; i < str.size() - 1; i++) {
            if (str[i] == '%') {
                if (i + 1 >= str.size() || str[i + 1] != 'd') {
                    pushError(line, 'a');
                    break;
                }
            } else if (str[i] == '\\') {
                if (i + 1 >= str.size() || str[i + 1] != 'n') {
                    pushError(line, 'a');
                    break;
                }
            } else if (!(str[i] == 32 || str[i] == 33 || 40 <= str[i] && str[i] <= 126)) {
                pushError(line, 'a');
                break;
            }
        }
    }
}

void ErrorHandling::grammarError(const Symbol& sym, SYMBOL tkn) {
    if (tkn == SEMICN) {
        pushError(sym.row, 'i');
    } else if (tkn == RPARENT) {
        pushError(sym.row, 'j');
    } else if (tkn == RBRACK) {
        pushError(sym.row, 'k');
    }
}

void ErrorHandling::identRepeat(const Symbol& sym) {
    pushError(sym.row, 'b');
}

void ErrorHandling::identNotFound(const Symbol& sym) {
    pushError(sym.row, 'c');
}

void ErrorHandling::paramsNumNotMatch(const Symbol& sym) {
    pushError(sym.row, 'd');
}

void ErrorHandling::paramsTypeNotMatch(const Symbol& sym) {
    pushError(sym.row, 'e');
}

void ErrorHandling::funcNoNeedReturn(const Symbol& sym) {
    pushError(sym.row, 'f');
}

void ErrorHandling::funcNeedReturn(const Symbol& sym) {
    pushError(sym.row, 'g');
}

void ErrorHandling::constAssign(const Symbol& sym) {
    pushError(sym.row, 'h');
}

void ErrorHandling::printfError(const Symbol& sym) {
    pushError(sym.row, 'l');
}

void ErrorHandling::loopError(const Symbol& sym) {
    pushError(sym.row, 'm');
}

void ErrorHandling::output() {
    ofstream f("error.txt");
    sort(errors.begin(), errors.end());
    for (auto x : errors) {
        cout << x.first << " " << x.second << endl;
        f << x.first << " " << x.second << endl;
    }
}

