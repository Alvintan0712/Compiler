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
    return declTable.back().count(decl->getIdent().val);
}

bool Table::checkDecl(LVal* lval) {
    if (lval->getIdent().sym != IDENFR) return true;
    if (findDecl(lval)) return true;
    err->identNotFound(lval->getIdent());
    return false;
}

Decl* Table::findDecl(LVal *lval) {
    for (auto v = declTable.rbegin(); v != declTable.rend(); v++)
        if (v->count(lval->getIdent().val))
            return (*v)[lval->getIdent().val];
    return nullptr;
}

void Table::pushDecl(Decl* decl) {
    if (containsDecl(decl)) err->identRepeat(decl->getIdent());
    else declTable.back().insert(make_pair(decl->getIdent().val, decl));
}

void Table::pushBlock() {
    declTable.emplace_back();
    funcTable.emplace_back();
}

void Table::popBlock() {
    declTable.pop_back();
    funcTable.pop_back();
}

bool Table::containsFunc(Func* func) {
    return funcTable.back().count(func->getIdent().val);
}

bool Table::checkFunc(CallExp* func) {
    if (func->getIdent().sym != IDENFR) return true;

    // ERROR_C
    Func* funcDecl = findFunc(func);
    if (!funcDecl) {
        err->identNotFound(func->getIdent());
        return false;
    }

    // ERROR_D
    vector<Decl*> fParams = funcDecl->getParams();
    vector<Exp*> rParams = func->getParams();
    if (fParams.size() != rParams.size()) {
        err->paramsNumNotMatch(func->getIdent());
        return false;
    }

    // ERROR_E
    int n = fParams.size();
    for (int i = 0; i < n; i++) {
        Decl* f = fParams[i];
        Exp* r = rParams[i];
        if (f->getType() != r->evalType()) {
            err->paramsTypeNotMatch(func->getIdent());
            return false;
        }
    }

    return false;
}

Func *Table::findFunc(CallExp *func) {
    for (auto v = funcTable.rbegin(); v != funcTable.rend(); v++)
        if (v->count(func->getIdent().val))
            return (*v)[func->getIdent().val];
    return nullptr;
}

void Table::pushFunc(Func* func) {
    if (containsFunc(func)) err->identRepeat(func->getIdent());
    else funcTable.back().insert(make_pair(func->getIdent().val, func));
}
