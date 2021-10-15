//
// Created by tjan2 on 23/9/2021.
//

#include "LexicalAnalyzer.h"

int row = 1, col = 1;
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
    char c = nextChar();
    string sym = "";

    sym += c;
    if (isspace(c) || checkUnary(sym)) {
        return;
    } else if (c == '|') {
        c = nextChar();
        if (c == '|') {
            sym += c;
            symbols.push_back(Symbol(OR, sym, row, col));
        } else {
            cout << "unknown symbols" << endl;
        }
    } else if (c == '&') {
        c = nextChar();
        if (c == '&') {
            sym += c;
            symbols.push_back(Symbol(AND, sym, row, col));
        } else {
            cout << "unknown symbols" << endl;
        }
    } else if (c == '!') {
        c = nextChar();
        if (c == '=') {
            sym += c;
            symbols.push_back(Symbol(NEQ, sym, row, col));
        } else {
            symbols.push_back(Symbol(NOT, sym, row, col));
            ptr--;
        }
    } else if (c == '=' || c == '<' || c == '>') {
        c = nextChar();
        if (c == '=') {
            sym += c;
            if (sym == "==") symbols.push_back(Symbol(EQL, sym, row, col));
            else if (sym == "<=") symbols.push_back(Symbol(LEQ, sym, row, col));
            else if (sym == ">=") symbols.push_back(Symbol(GEQ, sym, row, col));
        } else {
            if (sym == "=") symbols.push_back(Symbol(ASSIGN, sym, row, col));
            else if (sym == "<") symbols.push_back(Symbol(LSS, sym, row, col));
            else if (sym == ">") symbols.push_back(Symbol(GRE, sym, row, col));
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
            symbols.push_back(Symbol(DIV, sym, row, col));
            ptr--;
        }
    } else if (c == '_' || isalpha(c)) {
        sym = "";
        while (c == '_' || isalpha(c) || isdigit(c)) {
            sym += c;
            c = nextChar();
        }
        ptr--;
        if (tokens.count(sym)) symbols.push_back(Symbol(tokens[sym], sym, row, col));
        else symbols.push_back(Symbol(IDENFR, sym, row, col));
    } else if (isdigit(c)) {
        bool iszero = c == '0';
        if (iszero && isdigit(viewNextChar())) {
            cout << "no zero prefix number" << endl;
            return;
        } else if (iszero) {
            symbols.push_back(Symbol(INTCON, sym, row, col));
            return;
        }
        sym = "";
        while (isdigit(c)) {
            sym += c;
            c = nextChar();
        }
        ptr--;
        symbols.push_back(Symbol(INTCON, sym, row, col));
    } else if (c == '\"') {
        do {
            c = nextChar();
            if (c == '\"' || c == 32 || c == 33 || 40 <= c && c <= 126) {
                sym += c;
            } else if (c == '%') {
                sym += c;
                c = nextChar();
                if (c == 'd') sym += c;
                else cout << "wrong format in FormatString" << endl;
            } else {
                cout << "wrong format in FormatString" << endl;
            }
        } while (c != '\"');
        symbols.push_back(Symbol(STRCON, sym, row, col));
    }
}

char LexicalAnalyzer::nextChar() {
    if (src[ptr] == '\n') {
        row++;
        col = 0;
    } else {
        col++;
    }
    return src[ptr] ? src[ptr++] : 0;
}

char LexicalAnalyzer::viewNextChar() {
    return src[ptr];
}

bool LexicalAnalyzer::checkUnary(string sym) {
    if (sym == "+") {
        symbols.push_back(Symbol(PLUS, sym, row, col));
        return true;
    } else if (sym == "-") {
        symbols.push_back(Symbol(MINU, sym, row, col));
        return true;
    } else if (sym == "*") {
        symbols.push_back(Symbol(MULT, sym, row, col));
        return true;
    } else if (sym == "%") {
        symbols.push_back(Symbol(MOD, sym, row, col));
        return true;
    } else if (sym == ";") {
        symbols.push_back(Symbol(SEMICN, sym, row, col));
        return true;
    } else if (sym == ",") {
        symbols.push_back(Symbol(COMMA, sym, row, col));
        return true;
    } else if (sym == "(") {
        symbols.push_back(Symbol(LPARENT, sym, row, col));
        return true;
    } else if (sym == ")") {
        symbols.push_back(Symbol(RPARENT, sym, row, col));
        return true;
    } else if (sym == "[") {
        symbols.push_back(Symbol(LBRACK, sym, row, col));
        return true;
    } else if (sym == "]") {
        symbols.push_back(Symbol(RBRACK, sym, row, col));
        return true;
    } else if (sym == "{") {
        symbols.push_back(Symbol(LBRACE, sym, row, col));
        return true;
    } else if (sym == "}") {
        symbols.push_back(Symbol(RBRACE, sym, row, col));
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
