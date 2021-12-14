//
// Created by tjan2 on 17/11/2021.
//

#include "Context.h"
#include "IrFunc.h"
using namespace std;

IrFunc::IrFunc() {
    varId = 0;
}

IrFunc::IrFunc(Func *func) {
    varId = 0;
    returnType = func->getType();
    funcName   = func->getIdent().val;
}

void IrFunc::addBlock(BasicBlock *blk) {
    blocks.push_back(blk);
}

void IrFunc::addDecl(Decl* d, DeclInst* decl) {
    table[d] = decl->getVar();
}

void IrFunc::addVar(Decl *decl, Variable* v) {
    table[decl] = v;
}

int IrFunc::genVar() {
    return varId++;
}

std::string IrFunc::getName() {
    return funcName;
}

BasicBlock *IrFunc::getEntryBlock() const {
    return blocks.head;
}

std::vector<IrParam *> IrFunc::getParams() {
    return params;
}

int IrFunc::getVarId() const {
    return varId;
}

Variable *IrFunc::getVar(Decl *decl) {
    return table[decl];
}

Type IrFunc::getType() {
    return returnType;
}

std::string IrFunc::show() {
    string ty = returnType.getString();
    string f  = ty + " " + funcName + "()";
    string p;
    for (auto x : params) p += x->show() + "\n";
    string b;
    for (BasicBlock* blk = blocks.head; blk; blk = blk->next) {
        b += blk->show();
    }
    return f + "\n" + p + b;
}

int IrFunc::genArray(int size) {
    int id = varId;
    varId += size;
    return id;
}
