//
// Created by tjan2 on 17/11/2021.
//

#ifndef SRC_MODULE_H
#define SRC_MODULE_H

#include <map>
#include <set>
#include "IrFunc.h"

class IrFunc;
class DeclInst;
class Module {
public:
    std::vector<DeclInst*> decls;
    std::vector<IrFunc*> irFuncs;
    std::set<std::string> strings;

    Module();
    void addFunc(IrFunc* func);
    void addDecl(Decl* d, DeclInst* decl);
    void addStr(const std::string& str);

    IrFunc* getFunc(const std::string& name);
    Variable* getGlobal(Decl* d);
    void show();
private:
    std::map<std::string, IrFunc*> funcs;
    std::map<Decl*, Variable*> globals;
};


#endif //SRC_MODULE_H
