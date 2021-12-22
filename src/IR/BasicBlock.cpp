//
// Created by tjan2 on 17/11/2021.
//

#include "BasicBlock.h"

BasicBlock::BasicBlock() {
    this->prev = nullptr;
    this->next = nullptr;
    this->label_id = 0;
}

BasicBlock::BasicBlock(int id) {
    this->prev = nullptr;
    this->next = nullptr;
    this->label_id = id;
}

void BasicBlock::addPred(BasicBlock *pred) {
    this->pred.push_back(pred);
}

void BasicBlock::addSucc(BasicBlock *succ) {
    this->succ.push_back(succ);
}

void BasicBlock::addInst(Inst *inst) {
    insts.push_back(inst);
}

int BasicBlock::getId() const {
    return label_id;
}

Inst *BasicBlock::getLastInst() const {
    return insts.tail;
}

std::string BasicBlock::show() {
    std::string res;
    res += "label_" + std::to_string(label_id) + ":\n";
    for (Inst* inst = insts.head; inst; inst = inst->next) {
        res += inst->show() + "\n";
    }
    return res;
}

Inst *BasicBlock::getEntryInst() const {
    return insts.head;
}

void BasicBlock::insertAfter(Inst *src, Inst *inst) {
    insts.insertBack(inst, src);
}

void BasicBlock::remove(Inst *inst) {
    insts.remove(inst);
}
