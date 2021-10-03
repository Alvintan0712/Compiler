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
    void CompUnit();
    void Decl();
    void ConstDecl();
    void BType();
    void ConstDef();
    void ConstInitVal();
    void VarDecl();
    void VarDef();
    void InitVal();
    void FuncDef();
    void MainFuncDef();
    void FuncType();
    void FuncFParams();
    void FuncFParam();
    void Block();
    void BlockItem();
    void Stmt();
    void Exp();
    void Cond();
    void LVal();
    void PrimaryExp();
    void Number();
    void UnaryExp();
    void UnaryOp();
    void FuncRParams();
    void MulExp();
    void AddExp();
    void RelExp();
    void EqExp();
    void LAndExp();
    void LOrExp();

    bool isExp();
    bool isBlockItem();
    void ConstExp();
    void Ident();
    void pushPair();

    void FormatString();
};


#endif //SRC_GRAMMARANALYZER_H
