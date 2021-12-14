//
// Created by tjan2 on 17/11/2021.
//

#include "Context.h"

Context::Context() {
    this->blk = nullptr;
    this->if_body_blk = nullptr;
    this->while_body_blk = nullptr;
    this->while_end_blk = nullptr;
    this->if_end_blk = nullptr;
    this->if_blk = nullptr;
    this->while_blk = nullptr;
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



void Context::pushWhile(BasicBlock *while_blk, BasicBlock *body_blk, BasicBlock *while_end_blk) {
    while_blks.push(while_blk);
    while_body_blks.push(body_blk);
    while_end_blks.push(while_end_blk);
    getWhile();
}

void Context::getWhile() {
    this->while_blk = while_blks.empty() ? nullptr : while_blks.top();
    this->while_body_blk = while_body_blks.empty() ? nullptr : while_body_blks.top();
    this->while_end_blk = while_end_blks.empty() ? nullptr : while_end_blks.top();
}

void Context::popWhile() {
    while_blks.pop();
    while_body_blks.pop();
    while_end_blks.pop();
    getWhile();
}

int Context::genArray(int size) {
    return global_id++;
}

