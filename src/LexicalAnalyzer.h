//
// Created by tjan2 on 23/9/2021.
//
#ifndef SRC_LEXICALANALYZER_H
#define SRC_LEXICALANALYZER_H

#include <string>
#include <vector>
#include <map>
#include "Symbol.h"

class LexicalAnalyzer {
public:
    LexicalAnalyzer(std::string buffer);
    void initialTokens();
    void analyze();
    void nextSymbol();
    char nextChar();
    bool checkUnary(std::string sym, int row, int col);
    char viewNextChar();
    void output();
    std::vector<Symbol> getSymbols();
private:
    int ptr;
    std::string src;
    std::map<std::string, SYMBOL> tokens;
    std::vector<Symbol> symbols;
};


#endif //SRC_LEXICALANALYZER_H
