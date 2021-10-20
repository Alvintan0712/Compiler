//
// Created by tjan2 on 1/10/2021.
//
#ifndef SRC_GRAMMARANALYZER_H
#define SRC_GRAMMARANALYZER_H

#include <bits/stdc++.h>
#include "Symbol.h"
using namespace std;

class GrammarAnalyzer {
public:
    GrammarAnalyzer(vector<pair<symbol, string>> symbols);
    void analyze();
    void output();
private:
    vector<pair<symbol, string>> src;
    vector<string> out;
    pair<symbol, string> sym;

    int ptr;
    pair<symbol, string> nextSymbol();
    pair<symbol, string> viewNextSymbol();

    pair<symbol, string> viewNextSymbol(int i);
    void _CompUnit();
    void _Decl();
    void _ConstDecl();
    void _BType();
    void _ConstDef();
    void _ConstInitVal();
    void _VarDecl();
    void _VarDef();
    void _InitVal();
    void _FuncDef();
    void _MainFuncDef();
    void _FuncType();
    void _FuncFParams();
    void _FuncFParam();
    void _Block();
    void _BlockItem();
    void _Stmt();
    void _Exp();
    void _Cond();
    void _LVal();
    void _PrimaryExp();
    void _Number();
    void _UnaryExp();
    void _UnaryOp();
    void _FuncRParams();
    void _MulExp();
    void _AddExp();
    void _RelExp();
    void _EqExp();
    void _LAndExp();
    void _LOrExp();

    bool isExp();
    bool isBlockItem();
    void _ConstExp();
    void _Ident();
    void pushPair();

    void FormatString();
};


#endif //SRC_GRAMMARANALYZER_H
