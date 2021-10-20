//
// Created by tjan2 on 15/10/2021.
//

#include <iostream>
#include <fstream>
#include <string>
#include "ErrorHandling.h"
using namespace std;

ErrorHandling::ErrorHandling() {}

void ErrorHandling::formatStringError(int line) {
    errors.push_back(to_string(line) + " a");
}

void ErrorHandling::grammarError(Symbol sym, SYMBOL tkn) {
    if (sym.sym == tkn) return;
    exit(0);
}

void ErrorHandling::output() {
    ofstream f("error.txt");
    for (auto x : errors)
        f << x << endl;
}

