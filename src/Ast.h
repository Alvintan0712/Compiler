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
    Decl(Type bType, Symbol ident);
    Decl(Type bType, Symbol ident, std::vector<Exp*> initVal);

    bool isConst();
    bool isParam();
    bool isPointer();
    int getDim();
    Type getType();
    Symbol getIdent();
    std::vector<int> getDims();
    std::vector<int> getInitVal();

    void traverse(int lev) override;
private:
    Type bType;
    Symbol ident;
    std::vector<Exp*> initVal;
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
    Block(Symbol lBrace, std::vector<BlockItem*> items, Symbol rBrace);

    Symbol getLBrace();
    Symbol getRBrace();

    ReturnStmt* evalReturn();
    std::vector<LoopStmt*> evalLoop();
    void checkLoop();
    void traverse(int lev) override;
private:
    std::vector<BlockItem*> block_items;
    Symbol lBrace, rBrace;
    bool loop;
};

class Func : public ProgramItem {
public:
    Func();
    Func(Type returnType, Symbol ident, std::vector<Decl*> fParams, Block* block);

    Type getType();
    Symbol getIdent();
    std::vector<Decl*> getParams();

    void checkLoop();
    void checkReturn();
    void traverse(int lev) override;
private:
    Symbol ident;
    Block *block;
    Type returnType;
    std::vector<Decl*> fParams;
};

class Exp {
public:
    Exp();

    bool isCond();
    void addCond();

    virtual int evalInt();
    virtual Type evalType();
    virtual void traverse(int lev);
private:
    bool cond;
};

class ExpStmt : public Stmt {
public:
    ExpStmt();
    explicit ExpStmt(Exp *e);

    void traverse(int lev) override;
private:
    Exp* exp;
};

class CondStmt : public Stmt {
public:
    CondStmt();
    CondStmt(Symbol token, Exp* condExp, Stmt* ifStmt, Stmt* elseStmt = nullptr);

    Symbol getToken();
    Stmt* getIfStmt();
    Stmt* getElseStmt();

    void traverse(int lev) override;
private:
    Symbol token;
    Exp* condExp;
    Stmt* ifStmt;
    Stmt* elseStmt;
};

class LoopStmt : public Stmt {
public:
    LoopStmt();
    explicit LoopStmt(Symbol sym);

    Symbol getSym();

    void traverse(int lev) override;
private:
    Symbol sym;
};

class ReturnStmt : public Stmt {
public:
    ReturnStmt();
    explicit ReturnStmt(Symbol sym, Exp* exp);

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
    explicit UnaryExp(Exp* exp);
    explicit UnaryExp(Symbol sym);
    explicit UnaryExp(Symbol op, Symbol sym);
    explicit UnaryExp(Symbol op, Exp* exp);

    Exp* getExp();
    Symbol getOp();

    virtual void traverse(int lev);
    virtual Type evalType();
    virtual int evalInt();
private:
    Symbol *op, sym;
    Exp* exp;
};

class BinaryExp : public Exp {
public:
    BinaryExp();
    BinaryExp(Exp* lhs, Symbol op, Exp* rhs);

    Symbol getVal();

    virtual int evalInt();
    virtual Type evalType();
    virtual void traverse(int lev);
private:
    Symbol op;
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
    explicit LVal(Symbol sym, std::vector<Exp *> dims);

    Symbol getIdent();

    int evalInt() override;
    Type evalType() override;
    void traverse(int lev) override;
private:
    Symbol ident;
    std::vector<Exp*> dims;
};

class AssignExp : public BinaryExp {
public:
    AssignExp();
    AssignExp(LVal* l, Exp* r);

    void checkConst();
    int evalInt() override;
    Type evalType() override;
    void traverse(int lev) override;
private:
    LVal* lhs;
    Exp* rhs;
};

class CallExp : public UnaryExp {
public:
    CallExp();
    explicit CallExp(Symbol ident);
    explicit CallExp(Symbol ident, std::vector<Exp*> rParams);

    Symbol getIdent();
    std::vector<Exp*> getParams();

    bool isGetInt() const;
    bool isPrintf() const;

    void checkPrintf();
    int evalInt() override;
    Type evalType() override;
    void traverse(int lev) override;
private:
    Symbol ident;
    std::vector<Exp*> rParams;
};

class Program {
public:
    Program();
    Program(std::vector<Decl*> decls, std::vector<Func*> funcs);

    void addError(ErrorHandling* error);

    void traverse(int lev);
    ErrorHandling* err;
private:
    std::vector<ProgramItem*> program_items;
};

#endif //SRC_AST_H
