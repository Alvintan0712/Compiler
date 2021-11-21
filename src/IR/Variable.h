//
// Created by tjan2 on 17/11/2021.
//

#ifndef SRC_VARIABLE_H
#define SRC_VARIABLE_H

#include "../FrontEnd/Type.h"
#include "../Ast.h"

class Decl;
class Variable {
public:
    Variable();
    Variable(Variable* var);
    Variable(int id, bool is_global, Type bType);

    int getId() const;
    bool isGlobal() const;
    Type getType();

    virtual std::string show();
private:
    int id;
    bool is_global;
    Type bType;

    std::string showDim();
};

class Constant : public Variable {
public:
    SYMBOL type;
    int value;
    std::string str;

    Constant();
    explicit Constant(int x);
    explicit Constant(std::string s);

    int getValue() const;
    std::string getStr() const;
    std::string show() override;
};

class IrParam : public Variable {
public:
    Constant* constant;

    IrParam();
    explicit IrParam(int varId, Decl* decl);
    IrParam(Variable* var);
    IrParam(Constant* constant);
    std::string show() override;
};


#endif //SRC_VARIABLE_H
