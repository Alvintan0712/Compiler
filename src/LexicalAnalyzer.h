//
// Created by tjan2 on 23/9/2021.
//
#include <bits/stdc++.h>
#include "Symbol.h"
using namespace std;

#ifndef SRC_LEXICALANALYZER_H
#define SRC_LEXICALANALYZER_H

class LexicalAnalyzer {
public:
    LexicalAnalyzer(string buffer);
    void initialSymbols();
    void initialTokens();
    void printSrc();
    void analyze();
    void nextSymbol();
    char nextChar();
    bool checkUnary(string sym);
    char viewNextChar();
    void output();
private:
    int ptr;
    string src;
    map<string, symbol> tokens;
    vector<pair<symbol, string>> symbols;
};


#endif //SRC_LEXICALANALYZER_H
