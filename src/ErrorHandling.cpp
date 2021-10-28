//
// Created by tjan2 on 15/10/2021.
//

#include <iostream>
#include <fstream>
#include <string>
#include "ErrorHandling.h"
using namespace std;

ErrorHandling::ErrorHandling() {

}

void ErrorHandling::pushError(int line, char err) {
    errors.emplace_back(to_string(line) + " " + err);
}

void ErrorHandling::formatStringError(Symbol sym) {
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
                if (i + 1 >= str.size() || str[i + 1] != '\n') {
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

void ErrorHandling::output() {
    ofstream f("error.txt");
    for (auto x : errors)
        f << x << endl;
}

void ErrorHandling::grammarError(Symbol sym, SYMBOL tkn) {

}

