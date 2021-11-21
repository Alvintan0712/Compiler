//
// Created by tjan2 on 15/10/2021.
//

#include <iostream>
#include <utility>
#include "Symbol.h"
using namespace std;

Symbol::Symbol() {

}

Symbol::Symbol(SYMBOL symbol, string value, int line, int column) {
    sym = symbol;
    val = move(value);
    row = line;
    col = column;
}

void Symbol::print() {
    cout << symbolsName[sym] << ' ' << val << ' ' << row << ' ' << col << endl;
}

bool Symbol::operator==(const Symbol &t) const {
    return sym == t.sym && val == t.val;
}

bool Symbol::operator!=(const Symbol &t) const {
    return sym != t.sym || val != t.val;
}
