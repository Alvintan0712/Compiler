//
// Created by tjan2 on 17/10/2021.
//

#ifndef SRC_AST_H
#define SRC_AST_H

#include <vector>
#include <string>
#include "Symbol.h"
#include "ErrorHandling.h"

class Node {
public:
    Node();
};

class ProgramItem : public Node {
public:
    ProgramItem();
    virtual void traverse(int lev);
};

class BlockItem : public Node {
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
    Symbol getType();
    void setConst();
    bool isConst();
    void addDim();
    void addDim(Exp* size);
    int getDim();
    void addParam();
    void setName(Symbol _name);
    Symbol getName();
    void addInitVal(Exp* init);
    void traverse(int lev) override;
private:
    std::vector<Exp*> initVal;
    std::vector<Exp*> dim;
    int level;
    bool Const, pointer, param;
    Symbol bType;
    Symbol name;
};

class Stmt : public BlockItem {
public:
    Stmt();
    virtual void traverse(int lev);
};

class Block : public Stmt {
public:
    Block();
    void addItem(BlockItem *item);
    void setLevel(int lev);
    int getLevel();
    void addLoop();
    bool isLoop();
    void traverse(int lev) override;
private:
    std::vector<BlockItem*> block_items;
    int level;
    bool loop;
};

class Func : public ProgramItem {
public:
    Func();
    Func(Symbol _returnType, Symbol _name, Block* _block);
    Func(Symbol _returnType, Symbol _name, Block* _block, std::vector<Decl*> v);
    void traverse(int lev) override;
private:
    Symbol returnType;
    Symbol name;
    std::vector<Decl*> FParams;
    Block *block;
};

class Exp : public Node {
public:
    Exp();
    void addCond();
    bool isCond();
    virtual void traverse(int lev);
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
private:
    Symbol val;
    Exp *lhs, *rhs;
};

class FormatString : public Exp {
public:
    FormatString();
    explicit FormatString(Symbol str);
    Symbol getSym();
    void traverse(int lev);
private:
    Symbol sym;
};

class LVal : public UnaryExp {
public:
    LVal();
    explicit LVal(Symbol sym);
    void addDim(Exp *exp);
    void traverse(int lev);
private:
    std::vector<Exp*> dims;
    Symbol name;
};

class AssignExp : public BinaryExp {
public:
    AssignExp();
    AssignExp(LVal* l, Exp* r);
    void traverse(int lev) override;
private:
    LVal* lhs;
    Exp* rhs;
};

class CallExp : public UnaryExp {
public:
    CallExp();
    explicit CallExp(Symbol f);
    void addParam(Exp* param);
    bool isGetInt();
    bool isPrintf();
    void traverse(int lev) override;
private:
    Symbol func;
    std::vector<Exp*> rParams;
};

class Program : public Node {
public:
    Program();
    void addItem(ProgramItem* item);
    std::vector<ProgramItem*> getItems();
    void traverse(int lev);
private:
    std::vector<ProgramItem*> program_items;
};

class Ast {
public:
    Ast();
    explicit Ast(Program* p, ErrorHandling* err);
    void traverse();
private:
    Program* program;
    ErrorHandling* errorHandling;
};

#endif //SRC_AST_H
