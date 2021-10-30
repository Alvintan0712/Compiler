//
// Created by tjan2 on 17/10/2021.
//

#ifndef SRC_AST_H
#define SRC_AST_H

#include <vector>
#include <string>
#include "Symbol.h"
#include "ErrorHandling.h"
#include "Type.h"

class Program;
class Ast {
public:
    Ast();
    explicit Ast(Program* p, ErrorHandling* err);
    void traverse();
private:
    Program* program;
};

class ProgramItem {
public:
    ProgramItem();
    virtual void traverse(int lev);
};

class BlockItem {
public:
    BlockItem();
    virtual void traverse(int lev);
};

class Exp;
class Decl : public ProgramItem, public BlockItem {
public:
    Decl();
    Decl(Symbol _bType, Symbol _name);
    void setType(Symbol _bType);
    Type getType();
    void setConst();
    bool isConst();
    void addDim();
    void addDim(Exp* size);
    int getDim();
    void addParam();
    void setName(Symbol _name);
    Symbol getName();
    void addInitVal(Exp* init);
    std::vector<int> getInitVal();
    void traverse(int lev) override;
private:
    std::vector<Exp*> initVal;
    int level;
    bool Const, pointer, param;
    Type bType;
    Symbol name;
};

class Stmt : public BlockItem {
public:
    Stmt();
    virtual void traverse(int lev);
};

class ReturnStmt;
class LoopStmt;
class Block : public Stmt {
public:
    Block();
    void addItem(BlockItem *item);
    void setLevel(int lev);
    int getLevel();
    void addLoop();
    bool isLoop();
    void setLBrace(Symbol sym);
    Symbol getLBrace();
    void setRBrace(Symbol sym);
    Symbol getRBrace();
    ReturnStmt* evalReturn();
    std::vector<LoopStmt*> evalLoop();
    void checkLoop();
    void traverse(int lev) override;
private:
    std::vector<BlockItem*> block_items;
    Symbol lBrace, rBrace;
    int level;
    bool loop;
};

class Func : public ProgramItem {
public:
    Func();
    Func(Symbol _returnType, Symbol _name, Block* _block);
    Func(Symbol _returnType, Symbol _name, Block* _block, std::vector<Decl*> v);
    Symbol getName();
    std::vector<Decl*> getParams();
    Type getType();
    void checkReturn();
    void checkLoop();
    void traverse(int lev) override;
private:
    Type returnType;
    Symbol name;
    std::vector<Decl*> FParams;
    Block *block;
};

class Exp {
public:
    Exp();
    void addCond();
    bool isCond();
    virtual void traverse(int lev);
    virtual Type evalType();
    virtual int evalInt();
private:
    bool cond;
};

class ExpStmt : public Stmt {
public:
    ExpStmt();
    void addExp(Exp* e);
    void traverse(int lev) override;
private:
    Exp* exp;
};

class CondStmt : public Stmt {
public:
    CondStmt();
    CondStmt(Symbol name, Exp* e, Stmt* stmt);
    void addElse(Stmt* stmt);
    Symbol getSym();
    Stmt* getIfStmt();
    Stmt* getElseStmt();
    void traverse(int lev) override;
private:
    Symbol sym;
    Exp* condExp;
    Stmt* ifStmt;
    Stmt* elseStmt;
};

class LoopStmt : public Stmt {
public:
    LoopStmt();
    explicit LoopStmt(Symbol name);
    Symbol getSym();
    void traverse(int lev) override;
private:
    Symbol sym;
};

class ReturnStmt : public Stmt {
public:
    ReturnStmt();
    explicit ReturnStmt(Symbol name);
    void addExp(Exp* e);
    Exp* getExp();
    Symbol getSymbol();
    void traverse(int lev) override;
private:
    Exp* exp;
    Symbol sym;
};

class UnaryExp : public Exp {
public:
    UnaryExp();
    void addOp(std::string _op);
    std::string getOp();
    void setExp(Exp* e);
    Exp* getExp();
    void addSym(Symbol name);
    virtual void traverse(int lev);
    virtual Type evalType();
    virtual int evalInt();
private:
    Symbol sym;
    Exp* exp;
    std::string op;
};

class BinaryExp : public Exp {
public:
    BinaryExp();
    void setVal(Symbol _val);
    Symbol getVal();
    void setLhs(Exp* node);
    void setRhs(Exp* node);
    virtual void traverse(int lev);
    virtual Type evalType();
    virtual int evalInt();
private:
    Symbol val;
    Exp *lhs, *rhs;
};

class FormatString : public Exp {
public:
    FormatString();
    explicit FormatString(Symbol str);
    Symbol getSym();
    void traverse(int lev) override;
private:
    Symbol sym;
};

class LVal : public UnaryExp {
public:
    LVal();
    explicit LVal(Symbol sym);
    void addDim(Exp *exp);
    Symbol getName();
    void traverse(int lev) override;
    Type evalType() override;
    int evalInt() override;
private:
    std::vector<Exp*> dims;
    Symbol name;
};

class AssignExp : public BinaryExp {
public:
    AssignExp();
    AssignExp(LVal* l, Exp* r);
    void checkConst();
    void traverse(int lev) override;
    Type evalType() override;
    int evalInt() override;
private:
    LVal* lhs;
    Exp* rhs;
};

class CallExp : public UnaryExp {
public:
    CallExp();
    explicit CallExp(Symbol f);
    void addParam(Exp* param);
    std::vector<Exp*> getParams();
    bool isGetInt();
    bool isPrintf();
    void checkPrintf();
    void traverse(int lev) override;
    Symbol getFunc();
    Type evalType() override;
    int evalInt() override;
private:
    Symbol func;
    std::vector<Exp*> rParams;
};

class Program {
public:
    Program();
    void addError(ErrorHandling* error);
    void addItem(ProgramItem* item);
    std::vector<ProgramItem*> getItems();
    void traverse(int lev);
    ErrorHandling* err;
private:
    std::vector<ProgramItem*> program_items;
};

#endif //SRC_AST_H
