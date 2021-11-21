//
// Created by tjan2 on 17/11/2021.
//

#ifndef SRC_CONTEXT_H
#define SRC_CONTEXT_H

#include "BasicBlock.h"
#include "../FrontEnd/Table.h"
#include "IrBuilder.h"
#include "../Ast.h"

class Table;
class IrFunc;
class Module;
class ExpStmt;
class Constant;
class IrBuilder;
class BasicBlock;
class Context {
public:
    Context();

    Table* table;
    IrFunc* func;
    Module* module;
    BasicBlock* blk;
    ExpStmt* expStmt;
    IrBuilder* irBuilder;
    bool isParam;

    void addConst(Decl* decl);
    void addLabel(int id, BasicBlock* blk);

    std::map<Decl*, Constant*> const_map;
    std::map<int, BasicBlock*> labels;
};


#endif //SRC_CONTEXT_H
