//
// Created by tjan2 on 23/9/2021.
//

#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(string buffer) {
    src = buffer;
    initialTokens();
    analyze();
}

void LexicalAnalyzer::initialTokens() {
    tokens["main"] = MIANTK;
    tokens["const"] = CCONSTTK;
    tokens["int"] = INTTK;
    tokens["break"] = BBREAKTK;
    tokens["continue"] = CCONTINUETK;
    tokens["if"] = IFFTK;
    tokens["else"] = EELSETK;
    tokens["while"] = WWHILETK;
    tokens["getint"] = GGETINTTK;
    tokens["printf"] = PRINTTK;
    tokens["return"] = RETURTK;
    tokens["void"] = VOTK;
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
            symbols.push_back(make_pair(OR, sym));
        } else {
            cout << "unknown symbols" << endl;
        }
    } else if (c == '&') {
        c = nextChar();
        if (c == '&') {
            sym += c;
            symbols.push_back(make_pair(ANDD, sym));
        } else {
            cout << "unknown symbols" << endl;
        }
    } else if (c == '!') {
        c = nextChar();
        if (c == '=') {
            sym += c;
            symbols.push_back(make_pair(NOTEQUAL, sym));
        } else {
            symbols.push_back(make_pair(NOTT, sym));
            ptr--;
        }
    } else if (c == '=' || c == '<' || c == '>') {
        c = nextChar();
        if (c == '=') {
            sym += c;
            if (sym == "==") symbols.push_back(make_pair(EQUAL, sym));
            else if (sym == "<=") symbols.push_back(make_pair(LEQUAL, sym));
            else if (sym == ">=") symbols.push_back(make_pair(GEEQ, sym));
        } else {
            if (sym == "=") symbols.push_back(make_pair(ASS, sym));
            else if (sym == "<") symbols.push_back(make_pair(LESS, sym));
            else if (sym == ">") symbols.push_back(make_pair(BGE, sym));
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
            symbols.push_back(make_pair(DIVV, sym));
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
        else symbols.push_back(make_pair(IDENFRR, sym));
    } else if (isdigit(c)) {
        bool iszero = c == '0';
        if (iszero && isdigit(viewNextChar())) {
            cout << "no zero prefix number" << endl;
            return;
        } else if (iszero) {
            symbols.push_back(make_pair(INTCONN, sym));
            return;
        }
        sym = "";
        while (isdigit(c)) {
            sym += c;
            c = nextChar();
        }
        ptr--;
        symbols.push_back(make_pair(INTCONN, sym));
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
        symbols.push_back(make_pair(STRCONN, sym));
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
        symbols.push_back(make_pair(PLUSS, sym));
        return true;
    } else if (sym == "-") {
        symbols.push_back(make_pair(MINUS, sym));
        return true;
    } else if (sym == "*") {
        symbols.push_back(make_pair(MUL, sym));
        return true;
    } else if (sym == "%") {
        symbols.push_back(make_pair(MOD, sym));
        return true;
    } else if (sym == ";") {
        symbols.push_back(make_pair(SEMICOLON, sym));
        return true;
    } else if (sym == ",") {
        symbols.push_back(make_pair(COMM, sym));
        return true;
    } else if (sym == "(") {
        symbols.push_back(make_pair(LPRT, sym));
        return true;
    } else if (sym == ")") {
        symbols.push_back(make_pair(RPRT, sym));
        return true;
    } else if (sym == "[") {
        symbols.push_back(make_pair(LBRK, sym));
        return true;
    } else if (sym == "]") {
        symbols.push_back(make_pair(RBRK, sym));
        return true;
    } else if (sym == "{") {
        symbols.push_back(make_pair(LBRE, sym));
        return true;
    } else if (sym == "}") {
        symbols.push_back(make_pair(RBRE, sym));
        return true;
    }
    return false;
}

void LexicalAnalyzer::printSrc() {
    cout << src << endl;
}

void LexicalAnalyzer::output() {
    const string symbolsName[] = {
            "UNKNOWN", "END", "IDENFR", "INTCON", "STRCON", "MAINTK", "CONSTTK",
            "INTTK", "BREAKTK", "CONTINUETK", "IFTK", "ELSETK", "NOT", "AND",
            "OR", "WHILETK", "GETINTTK", "PRINTFTK", "RETURNTK", "PLUS", "MINU",
            "VOIDTK", "MULT", "DIV", "MOD", "LSS", "LEQ", "GRE", "GEQ", "EQL",
            "NEQ", "ASSIGN", "SEMICN", "COMMA", "LPARENT", "RPARENT", "LBRACK",
            "RBRACK", "LBRACE", "RBRACE"
    };
    ofstream f("output.txt");
    for (auto x : symbols)
        f << symbolsName[x.first] << ' ' << x.second << endl;
}
