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

    Program* compUnit();
    std::vector<Decl*> decl();
    std::vector<Decl*> constDecl();
    Symbol bType();
    Decl* constDef(bool isConst, const Symbol& type);
    std::vector<Exp*> constInitVal();
    std::vector<Decl*> varDecl();
    Decl* varDef(const Symbol& type);
    std::vector<Exp*> initVal();
    Func* funcDef();
    Func* mainFuncDef();
    Symbol funcType();
    std::vector<Decl*> funcFParams();
    Decl* funcFParam();
    Block* block();
    void blockItem(std::vector<BlockItem*>& items);
    Stmt* stmt();
    Exp* exp();
    Exp* cond();
    UnaryExp* lVal();
    UnaryExp* primaryExp();
    UnaryExp* number();
    UnaryExp* unaryExp();
    Symbol unaryOp();
    std::vector<Exp*> funcRParams();
    Exp* mulExp();
    Exp* addExp();
    Exp* relExp();
    Exp* eqExp();
    Exp* lAndExp();
    Exp* lOrExp();
    Exp* constExp();

    bool isExp() const;
    bool isBlockItem();
    Symbol ident();
    void pushSymbol();

    Symbol formatString();
};


#endif //SRC_GRAMMARANALYZER_H
