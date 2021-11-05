//
// Created by tjan2 on 1/10/2021.
//
#ifndef SRC_GRAMMARANALYZER_H
#define SRC_GRAMMARANALYZER_H

#include <string>
#include <vector>
#include <map>
#include "Ast.h"
#include "Symbol.h"
#include "ErrorHandling.h"

class GrammarAnalyzer {
public:
    GrammarAnalyzer(std::vector<Symbol> symbols, ErrorHandling* errorHandling);
    void analyze();
    void output();
    Program* getProgram();
private:
    std::vector<Symbol> src;
    std::vector<std::string> out;
    Symbol sym;

    Program* program;
    ErrorHandling* err;

    int ptr;
    Symbol nextSymbol();
    Symbol viewNextSymbol();
    Symbol viewNextSymbol(int i);

    Program* _CompUnit();
    std::vector<Decl*> _Decl();
    std::vector<Decl*> _ConstDecl();
    Symbol _BType();
    void _ConstDef(Decl* node);
    void _ConstInitVal(Decl* node);
    std::vector<Decl*> _VarDecl();
    void _VarDef(Decl* node);
    void _InitVal(Decl* node);
    Func* _FuncDef();
    Func* _MainFuncDef();
    Symbol _FuncType();
    std::vector<Decl*> _FuncFParams();
    Decl* _FuncFParam();
    Block* _Block();
    void _BlockItem(Block *blk);
    Stmt* _Stmt();
    Exp* _Exp();
    Exp* _Cond();
    UnaryExp* _LVal();
    UnaryExp* _PrimaryExp();
    UnaryExp* _Number();
    UnaryExp* _UnaryExp();
    Symbol _UnaryOp();
    std::vector<Exp*> _FuncRParams();
    Exp* _MulExp();
    Exp* _AddExp();
    Exp* _RelExp();
    Exp* _EqExp();
    Exp* _LAndExp();
    Exp* _LOrExp();
    Exp* _ConstExp();

    bool isExp() const;
    bool isBlockItem();
    Symbol _Ident();
    void pushSymbol();

    Symbol _FormatString();
};


#endif //SRC_GRAMMARANALYZER_H
