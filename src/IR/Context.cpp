//
// Created by tjan2 on 17/11/2021.
//

#include "Context.h"

Context::Context() {
    this->blk = nullptr;
    this->func = nullptr;
    this->table = nullptr;
    this->module = nullptr;
    this->expStmt = nullptr;
    this->irBuilder = new IrBuilder();
    this->isParam = false;
}

void Context::addConst(Decl *decl) {
    const_map[decl] = new Constant(decl->getInitVal()[0]);
}

void Context::addLabel(int id, BasicBlock* blk) {
    labels[id] = blk;
}

