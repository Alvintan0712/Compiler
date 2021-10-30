//
// Created by tjan2 on 28/10/2021.
//

#include "Table.h"
using namespace std;

Table::Table() {

}

void Table::addError(ErrorHandling* error) {
    err = error;
}

bool Table::containsDecl(Decl* decl) {
    return declTable.back().count(decl->getName().val);
}

bool Table::checkDecl(LVal* lval) {
    if (lval->getName().sym != IDENFR) return true;
    if (findDecl(lval)) return true;
    err->identNotFound(lval->getName());
    return false;
}

Decl* Table::findDecl(LVal *lval) {
    for (auto v = declTable.rbegin(); v != declTable.rend(); v++)
        if (v->count(lval->getName().val))
            return (*v)[lval->getName().val];
    return nullptr;
}

void Table::pushDecl(Decl* decl) {
    if (containsDecl(decl)) err->identRepeat(decl->getName());
    else declTable.back().insert(make_pair(decl->getName().val, decl));
}

void Table::pushBlock() {
    declTable.push_back(map<string, Decl*>());
    funcTable.push_back(map<string, Func*>());
}

void Table::popBlock() {
    declTable.pop_back();
    funcTable.pop_back();
}

bool Table::containsFunc(Func* func) {
    return funcTable.back().count(func->getName().val);
}

bool Table::checkFunc(CallExp* func) {
    if (func->getFunc().sym != IDENFR) return true;

    // ERROR_C
    Func* funcDecl = findFunc(func);
    if (!funcDecl) {
        err->identNotFound(func->getFunc());
        return false;
    }

    // ERROR_D
    vector<Decl*> fParams = funcDecl->getParams();
    vector<Exp*> rParams = func->getParams();
    if (fParams.size() != rParams.size()) {
        err->paramsNumNotMatch(func->getFunc());
        return false;
    }

    // ERROR_E
    int n = fParams.size();
    for (int i = 0; i < n; i++) {
        Decl* f = fParams[i];
        Exp* r = rParams[i];
        if (!(f->getType() == r->evalType())) {
            err->paramsTypeNotMatch(func->getFunc());
            return false;
        }
    }

    return false;
}

Func *Table::findFunc(CallExp *func) {
    for (auto v = funcTable.rbegin(); v != funcTable.rend(); v++)
        if (v->count(func->getFunc().val))
            return (*v)[func->getFunc().val];
    return nullptr;
}

void Table::pushFunc(Func* func) {
    if (containsFunc(func)) err->identRepeat(func->getName());
    else funcTable.back().insert(make_pair(func->getName().val, func));
}
