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
    this->bType = var->bType;
}

Variable::Variable(int id, bool is_global, Type bType) {
    this->id = id;
    this->is_global = is_global;
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
    return tag + to_string(id) + showDim();
}

std::string Variable::showDim() {
    string res;
    auto dims = bType.getDims();
    for (auto x : dims) res += "[" + to_string(x) + "]";
    return res;
}

Constant::Constant() {
    this->value = 0;
}

Constant::Constant(int x) {
    this->value = x;
    this->type  = INTTK;
}

Constant::Constant(std::string s) {
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
