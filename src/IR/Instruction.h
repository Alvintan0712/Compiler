//
// Created by tjan2 on 17/11/2021.
//

#ifndef SRC_INSTRUCTION_H
#define SRC_INSTRUCTION_H


#include "../FrontEnd/Type.h"
#include "../Ast.h"
#include "IrFunc.h"

class Exp;
class IrFunc;
class BinaryExp;
class Inst {
public:
    Inst *prev, *next;
    Type bType;

    Inst();

    virtual std::string show();
};

enum BinaryOp {
    Add, Sub, Mul, Div, Mod, And, Or,
    Slt, Sle, Sgt, Sge, Seq, Sne
};

class BinaryInst : public Inst {
public:
    BinaryOp op;
    Variable *var, *lhs, *rhs;

    BinaryInst();
    explicit BinaryInst(Variable* var, BinaryExp* exp);
    BinaryInst(Variable* var, BinaryOp op, Exp* exp);
    BinaryInst(Variable* var, BinaryOp op, Variable* lhs, Variable* rhs);
    BinaryInst(Variable* var, Variable* lhs, Symbol op, Variable* rhs);

    std::string show() override;
};

class AssignInst : public Inst {
public:
    Variable *lhs, *rhs;

    AssignInst();
    AssignInst(Variable* lhs, Variable* rhs);

    std::string show() override;
};

enum BranchOp {
    Br, Blt, Ble, Bgt, Bge, Beq, Bne, Bnez, Beqz
};

class BranchInst : public Inst {
public:
    BranchOp op;
    Variable* var;
    int label_id;

    BranchInst();
    BranchInst(BranchOp op, Variable* var, int label_id);

    std::string show() override;
};

class JumpInst : public Inst {
public:
    int label_id;

    JumpInst();
    JumpInst(int label_id);

    std::string show() override;
};

class ReturnInst : public Inst {
public:
    Variable* var;

    ReturnInst();
    explicit ReturnInst(Variable* var);

    std::string show() override;
};

class CallInst : public Inst {
public:
    Symbol sym;
    IrFunc* func;
    std::vector<Variable*> params;

    CallInst();
    CallInst(Symbol sym);
    CallInst(Symbol sym, std::vector<Variable*>& params);
    CallInst(IrFunc* func);
    CallInst(IrFunc* func, std::vector<Variable*>& params);

    std::string show() override;
};

class NotInst : public Inst {
public:
    Variable *var, *not_var;
    NotInst();
    NotInst(Variable* var, Variable* not_var);

    std::string show() override;
};

class IrArray;
class LoadAddrInst : public Inst {
public:
    Variable *var, *base;

    LoadAddrInst();
    LoadAddrInst(Variable* var, Variable* base);

    std::string show() override;
};

class LoadInst : public Inst {
public:
    Variable *dst, *addr;

    LoadInst();
    LoadInst(Variable* dst, Variable* addr);

    std::string show() override;
};

class StoreInst : public Inst {
public:
    Variable *addr, *val;
    StoreInst();
    StoreInst(Variable* val, Variable* addr);

    std::string show() override;
};

class DeclInst : public Inst {
public:
    DeclInst();
    DeclInst(Variable* var);

    Variable* getVar();
    Variable* getInit();
    std::vector<Variable*> getInits();
    void addInit(Variable* init);
    void addInits(std::vector<Variable*> inits);
    bool hasInit();

    std::string show() override;
private:
    Variable *var, *init;
    std::vector<Variable*> inits;
};

class GetReturnInst : public Inst {
public:
    Variable* var;

    GetReturnInst();
    GetReturnInst(Variable* var);

    std::string show() override;
};

#endif //SRC_INSTRUCTION_H
