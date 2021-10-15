//
// Created by tjan2 on 23/9/2021.
//
#ifndef SRC_LEXICALANALYZER_H
#define SRC_LEXICALANALYZER_H

#include <bits/stdc++.h>
#include "Symbol.h"
using namespace std;

class LexicalAnalyzer {
public:
    LexicalAnalyzer(string buffer);
    void initialTokens();
    void analyze();
    void nextSymbol();
    char nextChar();
    bool checkUnary(string sym);
    char viewNextChar();
    void output();
    vector<Symbol> getSymbols();
private:
    int ptr;
    string src;
    map<string, SYMBOL> tokens;
    vector<Symbol> symbols;
};


#endif //SRC_LEXICALANALYZER_H
