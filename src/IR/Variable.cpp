//
// Created by tjan2 on 17/11/2021.
//

#include "Variable.h"

#include <utility>
using namespace std;

Variable::Variable() {

}

Variable::Variable(Variable *var) {
    this->id = var->id;
    this->is_global = var->is_global;
    this->is_addr = false;
    this->bType = var->bType;
}

Variable::Variable(int id, bool is_global, Type bType) {
    this->id = id;
    this->is_global = is_global;
    this->is_addr = false;
    this->bType = move(bType);
}

int Variable::getId() const {
    return id;
}

bool Variable::isGlobal() const {
    return is_global;
}

Type Variable::getType() {
    return bType;
}

string Variable::show() {
    string tag = isGlobal() ? "@" : "%";
    string addr = is_addr ? "la " : "";
    return addr + tag + to_string(id) + showDim();
}

void Variable::addDims(std::vector<Variable *> dims) {
    this->dims = dims;
}

std::string Variable::showDim() {
    string res;
    for (auto x : dims) res += "[" + x->show() + "]";
    return res;
}

void Variable::addAddr() {
    is_addr = true;
}

Constant::Constant() {
    this->value = 0;
    this->type = UNKNOWN;
}

Constant::Constant(int x) {
    this->value = x;
    this->type  = INTTK;
}

Constant::Constant(std::string s) {
    this->value = 0;
    this->str  = std::move(s);
    this->type = STRCON;
}

std::string Constant::show() {
    if (type == STRCON) return "\"" + str + "\"";
    return to_string(value);
}

int Constant::getValue() const {
    return value;
}

std::string Constant::getStr() const {
    return str;
}

IrParam::IrParam() {
    this->constant = nullptr;
}

IrParam::IrParam(int varId, Decl *decl) : Variable(varId, false, decl->getType()) {
    this->constant = nullptr;
}

IrParam::IrParam(Variable *var) : Variable(var) {
    this->constant = nullptr;
}

IrParam::IrParam(Constant* constant) {
    this->constant = constant;
}

std::string IrParam::show() {
    if (constant) return constant->show();
    return Variable::show();
}

IrArray::IrArray() {
    this->isConst = false;
    this->base = 0;
    this->size = 0;
}

void IrArray::addConst() {
    isConst = true;
}

IrArray::IrArray(int id, bool is_global, const Type& bType) : Variable(id, is_global, bType) {
    this->isConst = false;
    this->base = id; // array id
    auto dims = bType.getDims();
    this->size = 1;
    for (auto x : dims) this->size *= x;
}

std::string IrArray::show() {
    auto tag = isGlobal() ? "@" : "%";
    return tag + to_string(base);
}

int IrArray::getBase() const {
    return base;
}

int IrArray::getSize() const {
    return size;
}

IrPointer::IrPointer() {
    this->isConst = false;
    this->base = 0;
}

IrPointer::IrPointer(int base, Decl* decl) : IrParam(base, decl) {
    this->isConst = false;
    this->base = base;
}

void IrPointer::addConst() {
    isConst = true;
}

int IrPointer::getBase() const {
    return base;
}

std::string IrPointer::show() {
    string res = "%" + to_string(base) + "[]";
    auto dims = getType().getDims();
    for (auto x : dims) res += "[" + to_string(x) + "]";
    return res;
}
