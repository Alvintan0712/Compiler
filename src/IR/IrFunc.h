//
// Created by tjan2 on 17/11/2021.
//

#ifndef SRC_IRFUNC_H
#define SRC_IRFUNC_H

#include <vector>
#include <map>
#include "IntrusiveLinkedList.h"
#include "BasicBlock.h"
#include "Variable.h"
#include "Instruction.h"

class BasicBlock;
class DeclInst;
class Variable;
class IrParam;
class Func;
class Decl;
class IrFunc {
public:
    IrFunc();
    explicit IrFunc(Func* func);

    void addBlock(BasicBlock* blk);
    void addDecl(Decl* d, DeclInst* decl);
    void addVar(Decl* decl, Variable* v);
    int genVar();

    int getVarId() const;
    Type getType();
    std::string getName();
    Variable* getVar(Decl* decl);
    std::vector<IrParam*> getParams();
    BasicBlock* getEntryBlock() const;

    std::string show();
private:
    int varId;

    IntrusiveLinkedList<BasicBlock> blocks;
    std::string funcName;
    std::vector<IrParam*> params;
    std::map<Decl*, Variable*> table;
    Type returnType;
};


#endif //SRC_IRFUNC_H
