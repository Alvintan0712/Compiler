//
// Created by tjan2 on 1/10/2021.
//
#ifndef SRC_GRAMMARANALYZER_H
#define SRC_GRAMMARANALYZER_H

#include <string>
#include <vector>
#include <map>
#include "Symbol.h"
#include "ErrorHandling.h"

class GrammarAnalyzer {
public:
    GrammarAnalyzer(std::vector<Symbol> symbols);
    void analyze();
    void output();
private:
    std::vector<Symbol> src;
    std::vector<std::string> out;
    Symbol sym;

    ErrorHandling error;
    std::vector<std::map<std::string, Symbol>> IdentTable;

    int ptr;
    Symbol nextSymbol();
    Symbol viewNextSymbol();
    Symbol viewNextSymbol(int i);

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
    void pushSymbol();

    void _FormatString();
};


#endif //SRC_GRAMMARANALYZER_H
