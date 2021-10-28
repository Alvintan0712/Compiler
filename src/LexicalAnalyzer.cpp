//
// Created by tjan2 on 23/9/2021.
//

#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(string buffer) {
    src = buffer;
    initialTokens();
    analyze();
}

bool charMode = false;
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
    tokens["char"] = CHARTK;
}

void LexicalAnalyzer::analyze() {
    ptr = 0;
    while (src[ptr])
        nextSymbol();
}

void LexicalAnalyzer::nextSymbol() {
    char c = nextChar();
    string sym = "";

    sym += c;
    if (isspace(c) || checkUnary(sym)) {
        return;
    } else if (charMode) {
        if (c == 32 || c == 33 || 40 <= c && c <= 126) {
            symbols.push_back(make_pair(CHAR, sym));
        }
    } else if (c == '|') {
        c = nextChar();
        if (c == '|') {
            sym += c;
            symbols.push_back(make_pair(OR, sym));
        } else {
            cout << "unknown symbols" << endl;
        }
    } else if (c == '&') {
        c = nextChar();
        if (c == '&') {
            sym += c;
            symbols.push_back(make_pair(AND, sym));
        } else {
            cout << "unknown symbols" << endl;
        }
    } else if (c == '!') {
        c = nextChar();
        if (c == '=') {
            sym += c;
            symbols.push_back(make_pair(NEQ, sym));
        } else {
            symbols.push_back(make_pair(NOT, sym));
            ptr--;
        }
    } else if (c == '=' || c == '<' || c == '>') {
        c = nextChar();
        if (c == '=') {
            sym += c;
            if (sym == "==") symbols.push_back(make_pair(EQL, sym));
            else if (sym == "<=") symbols.push_back(make_pair(LEQ, sym));
            else if (sym == ">=") symbols.push_back(make_pair(GEQ, sym));
        } else {
            if (sym == "=") symbols.push_back(make_pair(ASSIGN, sym));
            else if (sym == "<") symbols.push_back(make_pair(LSS, sym));
            else if (sym == ">") symbols.push_back(make_pair(GRE, sym));
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
            symbols.push_back(make_pair(DIV, sym));
            ptr--;
        }
    } else if (c == '_' || isalpha(c)) {
        sym = "";
        while (c == '_' || isalpha(c) || isdigit(c)) {
            sym += c;
            c = nextChar();
        }
        ptr--;
        if (tokens.count(sym)) symbols.push_back(make_pair(tokens[sym], sym));
        else symbols.push_back(make_pair(IDENFR, sym));
    } else if (isdigit(c)) {
        bool iszero = c == '0';
        if (iszero && isdigit(viewNextChar())) {
            cout << "no zero prefix number" << endl;
            return;
        } else if (iszero) {
            symbols.push_back(make_pair(INTCON, sym));
            return;
        }
        sym = "";
        while (isdigit(c)) {
            sym += c;
            c = nextChar();
        }
        ptr--;
        symbols.push_back(make_pair(INTCON, sym));
    } else if (c == '\"') {
        do {
            c = nextChar();
            if (c == '\"' || c == 32 || c == 33 || 40 <= c && c <= 126) {
                sym += c;
            } else if (c == '%') {
                sym += c;
                c = nextChar();
                if (c == 'd' || c == 'c') sym += c;
                else cout << "wrong format in FormatString" << endl;
            } else {
                cout << "wrong format in FormatString" << endl;
            }
        } while (c != '\"');
        symbols.push_back(make_pair(STRCON, sym));
    }
}

char LexicalAnalyzer::nextChar() {
    return src[ptr] ? src[ptr++] : 0;
}

char LexicalAnalyzer::viewNextChar() {
    return src[ptr];
}

bool LexicalAnalyzer::checkUnary(string sym) {
    if (sym == "+") {
        symbols.push_back(make_pair(PLUS, sym));
        return true;
    } else if (sym == "-") {
        symbols.push_back(make_pair(MINU, sym));
        return true;
    } else if (sym == "*") {
        symbols.push_back(make_pair(MULT, sym));
        return true;
    } else if (sym == "%") {
        symbols.push_back(make_pair(MOD, sym));
        return true;
    } else if (sym == ";") {
        symbols.push_back(make_pair(SEMICN, sym));
        return true;
    } else if (sym == ",") {
        symbols.push_back(make_pair(COMMA, sym));
        return true;
    } else if (sym == "(") {
        symbols.push_back(make_pair(LPARENT, sym));
        return true;
    } else if (sym == ")") {
        symbols.push_back(make_pair(RPARENT, sym));
        return true;
    } else if (sym == "[") {
        symbols.push_back(make_pair(LBRACK, sym));
        return true;
    } else if (sym == "]") {
        symbols.push_back(make_pair(RBRACK, sym));
        return true;
    } else if (sym == "{") {
        symbols.push_back(make_pair(LBRACE, sym));
        return true;
    } else if (sym == "}") {
        symbols.push_back(make_pair(RBRACE, sym));
        return true;
    } else if (sym == "\'") {
        symbols.push_back(make_pair(SINGLEQUOTE, sym));
        charMode = !charMode;
        return true;
    }
    return false;
}

void LexicalAnalyzer::output() {
    ofstream f("output.txt");
    for (auto x : symbols)
        cout << symbolsName[x.first] << ' ' << x.second << endl;
}

vector<pair<symbol, string>> LexicalAnalyzer::getSymbols() {
    return symbols;
}
