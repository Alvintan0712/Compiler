//
// Created by tjan2 on 23/9/2021.
//

#include "LexicalAnalyzer.h"
#include <iostream>
using namespace std;

int row = 1, column = 1;
LexicalAnalyzer::LexicalAnalyzer(string buffer) {
    src = buffer;
    initialTokens();
    analyze();
}

void LexicalAnalyzer::initialTokens() {
    tokens["main"] = MAINTK;
    tokens["const"] = CONSTTK;
    tokens["int"] = INTTK;
    tokens["break"] = BREAKTK;
    tokens["continue"] = CONTINUETK;
    tokens["if"] = IFTK;
    tokens["else"] = ELSETK;
    tokens["while"] = WHILETK;
    tokens["getint"] = GETINTTK;
    tokens["printf"] = PRINTFTK;
    tokens["return"] = RETURNTK;
    tokens["void"] = VOIDTK;
}

void LexicalAnalyzer::analyze() {
    ptr = 0;
    while (src[ptr]) nextSymbol();
}

void LexicalAnalyzer::nextSymbol() {
    int r = row, col = column;
    char c = nextChar();
    string sym = "";

    sym += c;
    if (isspace(c) || checkUnary(sym, r, col)) {
        return;
    } else if (c == '|') {
        c = nextChar();
        if (c == '|') {
            sym += c;
            symbols.emplace_back(Symbol(OR, sym, r, col));
        } else {
            cout << "unknown symbols" << endl;
        }
    } else if (c == '&') {
        c = nextChar();
        if (c == '&') {
            sym += c;
            symbols.emplace_back(Symbol(AND, sym, r, col));
        } else {
            cout << "unknown symbols" << endl;
        }
    } else if (c == '!') {
        c = nextChar();
        if (c == '=') {
            sym += c;
            symbols.emplace_back(Symbol(NEQ, sym, r, col));
        } else {
            symbols.emplace_back(Symbol(NOT, sym, r, col));
            ptr--;
        }
    } else if (c == '=' || c == '<' || c == '>') {
        c = nextChar();
        if (c == '=') {
            sym += c;
            if (sym == "==") symbols.emplace_back(Symbol(EQL, sym, r, col));
            else if (sym == "<=") symbols.emplace_back(Symbol(LEQ, sym, r, col));
            else if (sym == ">=") symbols.emplace_back(Symbol(GEQ, sym, r, col));
        } else {
            if (sym == "=") symbols.emplace_back(Symbol(ASSIGN, sym, r, col));
            else if (sym == "<") symbols.emplace_back(Symbol(LSS, sym, r, col));
            else if (sym == ">") symbols.emplace_back(Symbol(GRE, sym, r, col));
            ptr--;
        }
    } else if (c == '/') {
        c = nextChar();
        if (c == '/') {
            while (c != '\n' && c != '\0')
                c = nextChar();
        } else if (c == '*') {
            char cc;
            c = nextChar();
            cc = c; c = nextChar();
            while (cc != '*' || c != '/')
                cc = c, c = nextChar();
        } else {
            symbols.emplace_back(Symbol(DIV, sym, r, col));
            ptr--;
        }
    } else if (c == '_' || isalpha(c)) {
        sym = "";
        while (c == '_' || isalpha(c) || isdigit(c)) {
            sym += c;
            c = nextChar();
        }
        if (c == '\n') row--;
        ptr--;
        if (tokens.count(sym)) symbols.emplace_back(Symbol(tokens[sym], sym, r, col));
        else symbols.emplace_back(Symbol(IDENFR, sym, r, col));
    } else if (isdigit(c)) {
        bool iszero = c == '0';
        if (iszero && isdigit(viewNextChar())) {
            cout << "no zero prefix number" << endl;
            return;
        } else if (iszero) {
            symbols.emplace_back(Symbol(INTCON, sym, r, col));
            return;
        }
        sym = "";
        while (isdigit(c)) {
            sym += c;
            c = nextChar();
        }
        ptr--;
        symbols.emplace_back(Symbol(INTCON, sym, r, col));
    } else if (c == '\"') {
        do {
            c = nextChar();
            sym += c;
        } while (c != '\"');
        symbols.emplace_back(Symbol(STRCON, sym, r, col));
    }
}

char LexicalAnalyzer::nextChar() {
    if (src[ptr] == '\n') {
        row++;
        column = 1;
    } else {
        column++;
    }
    return src[ptr] ? src[ptr++] : 0;
}

char LexicalAnalyzer::viewNextChar() {
    return src[ptr];
}

bool LexicalAnalyzer::checkUnary(string sym, int row, int column) {
    if (sym == "+") {
        symbols.emplace_back(Symbol(PLUS, sym, row, column));
        return true;
    } else if (sym == "-") {
        symbols.emplace_back(Symbol(MINU, sym, row, column));
        return true;
    } else if (sym == "*") {
        symbols.emplace_back(Symbol(MULT, sym, row, column));
        return true;
    } else if (sym == "%") {
        symbols.emplace_back(Symbol(MOD, sym, row, column));
        return true;
    } else if (sym == ";") {
        symbols.emplace_back(Symbol(SEMICN, sym, row, column));
        return true;
    } else if (sym == ",") {
        symbols.emplace_back(Symbol(COMMA, sym, row, column));
        return true;
    } else if (sym == "(") {
        symbols.emplace_back(Symbol(LPARENT, sym, row, column));
        return true;
    } else if (sym == ")") {
        symbols.emplace_back(Symbol(RPARENT, sym, row, column));
        return true;
    } else if (sym == "[") {
        symbols.emplace_back(Symbol(LBRACK, sym, row, column));
        return true;
    } else if (sym == "]") {
        symbols.emplace_back(Symbol(RBRACK, sym, row, column));
        return true;
    } else if (sym == "{") {
        symbols.emplace_back(Symbol(LBRACE, sym, row, column));
        return true;
    } else if (sym == "}") {
        symbols.emplace_back(Symbol(RBRACE, sym, row, column));
        return true;
    }
    return false;
}

void LexicalAnalyzer::output() {
    for (auto x : symbols)
        x.print();
}

vector<Symbol> LexicalAnalyzer::getSymbols() {
    return symbols;
}
