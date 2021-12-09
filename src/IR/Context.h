//
// Created by tjan2 on 17/11/2021.
//

#ifndef SRC_CONTEXT_H
#define SRC_CONTEXT_H

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
    BasicBlock *blk, *if_blk, *while_blk, *body_blk, *else_blk, *while_end_blk, *if_end_blk;
    ExpStmt* expStmt;
    bool isParam;
    bool isCond;

    void addConst(Decl* decl);
    void addLabel(int id, BasicBlock* blk);
    int genGlobal();
    int genLabel();
    IrFunc* getFunc(const std::string& name);

    std::map<Decl*, Constant*> const_map;
    std::map<int, BasicBlock*> labels;
private:
    int global_id, label_id;
};


#endif //SRC_CONTEXT_H
