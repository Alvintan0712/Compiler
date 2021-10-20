//
// Created by tjan2 on 17/10/2021.
//

#ifndef SRC_AST_H
#define SRC_AST_H

#include <vector>
#include <string>
#include "Symbol.h"

class ProgramItem {
public:
    ProgramItem();
};

class BlockItem {
public:
    BlockItem();
};

class Decl : public ProgramItem, public BlockItem {
public:
    Decl();
    Decl(SYMBOL _bType, std::string _name, int _level);
    void setType(SYMBOL _bType);
    SYMBOL getType();
    void setConst();
    bool isConst();
    void addDim(int size);
    int getDim();
    std::string getName();
private:
    std::vector<int> dim;
    int level;
    bool Const;
    SYMBOL bType;
    std::string name;
};

class Block {
public:
    Block();
    Block(int _level);
    void addItem(BlockItem item);
private:
    std::vector<BlockItem*> block_items;
    int level;
};

class Func : public ProgramItem {
public:
    Func();
    Func(SYMBOL _returnType, std::string _name, Block _block);
private:
    SYMBOL returnType;
    std::string name;
    std::vector<Decl*> FParams;
    Block *block;
};

class Stmt : public BlockItem {
public:
    Stmt();
};

class Exp : public Stmt {
public:
    Exp();
};

class LVal : public Exp {
public:
    LVal();
private:
    std::vector<int> dims;
    std::string name;
};

class AssignExp : public BinaryExp {
public:
    AssignExp();
private:
    LVal* lhs;
    Exp* rhs;
};

class UnaryExp : public Exp {
public:
    UnaryExp();
private:
    std::string op;

};

class BinaryExp : public Exp {
public:
    BinaryExp();
private:
    char op;
    Exp* lhs, rhs;
};

class CallExp : public UnaryExp {
public:
    CallExp();
private:
    std::string name;
};

class Program {
public:
    void addItem(ProgramItem item);
    std::vector<ProgramItem*> getItems();
private:
    std::vector<ProgramItem*> program_items;
};

class Ast {

};

#endif //SRC_AST_H
