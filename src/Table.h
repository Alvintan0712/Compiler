//
// Created by tjan2 on 28/10/2021.
//

#ifndef SRC_TABLE_H
#define SRC_TABLE_H

#include <set>
#include <vector>
#include <map>
#include "Symbol.h"
#include "Ast.h"

class Decl;
class LVal;
class Func;
class CallExp;
class Table {
public:
    Table();
    void addError(ErrorHandling* error);
    bool containsDecl(Decl* decl);
    bool checkDecl(LVal* lval);
    Decl* findDecl(LVal* lval);
    void pushDecl(Decl* decl);
    bool containsFunc(Func* func);
    bool checkFunc(CallExp* func);
    Func* findFunc(CallExp* func);
    void pushFunc(Func* func);
    void pushBlock();
    void popBlock();
private:
    ErrorHandling* err;
    std::vector<std::map<std::string, Decl*>> declTable;
    std::vector<std::map<std::string, Func*>> funcTable;
};


#endif //SRC_TABLE_H
