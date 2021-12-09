//
// Created by tjan2 on 17/11/2021.
//

#include "Context.h"

Context::Context() {
    this->blk = nullptr;
    this->body_blk = nullptr;
    this->end_blk = nullptr;
    this->cond_blk = nullptr;
    this->func = nullptr;
    this->table = nullptr;
    this->module = nullptr;
    this->expStmt = nullptr;
    this->isParam = false;
    this->isCond = false;

    this->global_id = 1;
    this->label_id = 1;
}

void Context::addConst(Decl *decl) {
    const_map[decl] = new Constant(decl->getInitVal()[0]);
}

void Context::addLabel(int id, BasicBlock* blk) {
    labels[id] = blk;
}

int Context::genGlobal() {
    return global_id++;
}

int Context::genLabel() {
    return label_id++;
}

IrFunc *Context::getFunc(const std::string &name) {
    return module->getFunc(name);
}

