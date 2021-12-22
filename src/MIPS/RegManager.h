//
// Created by alvin on 12/18/21.
//

#ifndef SRC_REGMANAGER_H
#define SRC_REGMANAGER_H


#include "../IR/Variable.h"

enum RegSymbol {
    zero,
    at,
    v0, v1,
    a0, a1, a2, a3,
    t0, t1, t2, t3, t4, t5, t6, t7,
    s0, s1, s2, s3, s4, s5, s6, s7,
    t8, t9,
    k0, k1,
    gp, sp, fp,
    ra
};


class Reg {
public:
    Reg();
    void setVar(Variable* var);
    void addId(int id);
    Variable* getVar();
    void setDirty();
    bool getDirty() const;
    bool dirty, free;

    std::string show();
private:
    int id;
    Variable* var;
};


class RegManager {
public:
    explicit RegManager(std::vector<std::string>* mips);
    Reg* allocReg();
    void freeReg();
    void setReg(Reg* reg, Variable* var);
    Reg* getReg(Variable* var);
    Reg* insertVar(Variable* var);
private:
    std::string tab = "    ";

    std::vector<std::string> *mips;
    Reg regs[32];
    std::map<Reg*, Variable*> regMap;
    std::vector<Reg*> freeRegs;
    std::vector<Reg*> useRegs;
};


#endif //SRC_REGMANAGER_H
