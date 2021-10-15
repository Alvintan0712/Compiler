//
// Created by tjan2 on 15/10/2021.
//

#include "Symbol.h"

Symbol::Symbol(SYMBOL symbol, string value, int line, int column) {
    sym = symbol;
    val = value;
    row = line;
    col = column;
}

void Symbol::print() {
    cout << symbolsName[sym] << ' ' << val << ' ' << row << endl;
}
