//
// Created by tjan2 on 17/11/2021.
//

#include <iostream>
#include <fstream>
#include "Module.h"
using namespace std;

Module::Module() {

}

void Module::addFunc(IrFunc *func) {
    funcs[func->getName()] = func;
    irFuncs.insert(irFuncs.begin(), 1, func);
}

IrFunc* Module::getFunc(const string& name) {
    return funcs[name];
}

void Module::addDecl(Decl* d, DeclInst* decl) {
    globals[d] = decl->getVar();
    decls.push_back(decl);
}

void Module::addStr(const std::string& str) {
    if (strings.count(str) == 0)
        strings.insert(str);
}

Variable *Module::getGlobal(Decl* d) {
    return globals[d];
}

void Module::show() {
    ofstream f("ir.txt");
    for (auto x : decls) {
        f << x->show() << endl;
        cout << x->show() << endl;
    }
    for (auto x : irFuncs) {
        f << x->show() << endl;
        cout << x->show() << endl;
    }
}

