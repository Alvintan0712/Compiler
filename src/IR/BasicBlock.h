//
// Created by tjan2 on 17/11/2021.
//

#ifndef SRC_BASICBLOCK_H
#define SRC_BASICBLOCK_H

#include "Instruction.h"
#include "IntrusiveLinkedList.h"

class Inst;
class BasicBlock {
public:
    BasicBlock *prev, *next;

    BasicBlock();
    explicit BasicBlock(int id);

    void addPred(BasicBlock* pred);
    void addSucc(BasicBlock* succ);
    void addInst(Inst* inst);
    void remove(Inst* inst);
    void insertAfter(Inst* src, Inst* inst);

    Inst* getEntryInst() const;
    Inst* getLastInst() const;
    int getId() const;

    std::string show();
private:
    int label_id;

    IntrusiveLinkedList<Inst> insts;
    std::vector<BasicBlock*> pred, succ;
};


#endif //SRC_BASICBLOCK_H
