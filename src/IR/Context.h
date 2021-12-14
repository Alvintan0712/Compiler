//
// Created by tjan2 on 17/11/2021.
//

#ifndef SRC_CONTEXT_H
#define SRC_CONTEXT_H

#include <stack>
#include "BasicBlock.h"
#include "../FrontEnd/Table.h"
#include "../Ast.h"

class Table;
class IrFunc;
class Module;
class ExpStmt;
class Constant;
class BasicBlock;
class Context {
public:
    Context();

    Table* table;
    IrFunc* func;
    Module* module;
    BasicBlock *blk;
    BasicBlock *if_blk;
    BasicBlock *if_body_blk;
    BasicBlock *else_blk;
    BasicBlock *if_end_blk;
    BasicBlock *while_blk;
    BasicBlock *while_body_blk;
    BasicBlock *while_end_blk;
    ExpStmt* expStmt;
    bool isParam;
    bool isCond;

    void addConst(Decl* decl);
    void addLabel(int id, BasicBlock* blk);
    int genGlobal();
    int genArray(int size);
    int genLabel();
    IrFunc* getFunc(const std::string& name);
    void pushWhile(BasicBlock *while_blk, BasicBlock *body_blk, BasicBlock *while_end_blk);
    void popWhile();

    std::map<Decl*, Constant*> const_map;
    std::map<int, BasicBlock*> labels;
private:
    std::stack<BasicBlock *> while_blks;
    std::stack<BasicBlock *> while_body_blks;
    std::stack<BasicBlock *> while_end_blks;
    std::stack<BasicBlock*> if_blks;
    std::stack<BasicBlock *> if_body_blks;
    std::stack<BasicBlock *> else_blks;
    std::stack<BasicBlock *> if_end_blks;
    int global_id, label_id;

    void getWhile();
};


#endif //SRC_CONTEXT_H
