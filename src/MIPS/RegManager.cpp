//
// Created by alvin on 12/18/21.
//

#include "RegManager.h"
using namespace std;

Reg::Reg() {
    this->id = -1;
    this->var = nullptr;
    this->free = true;
    this->dirty = false;
}

void Reg::addId(int id) {
    this->id = id;
}

void Reg::setVar(Variable *var) {
    this->var = var;
}

Variable *Reg::getVar() {
    return var;
}

void Reg::setDirty() {
    dirty = true;
}

bool Reg::getDirty() const {
    return dirty;
}

std::string Reg::show() {
    std::string sym[] = {
        "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
        "t8", "t9", "k0", "k1",
        "gp", "sp", "fp", "ra"
    };
    return "$" + sym[id];
}

RegManager::RegManager(std::vector<std::string>* mips) {
    this->mips = mips;

    regs[zero].free = false;
    regs[at].free = false;
    regs[sp].free = false;
    regs[ra].free = false;
    regs[v0].free = false;
    regs[a0].free = false;

    for (int i = 0; i < 32; i++) {
        regs[i].addId(i);
        if (regs[i].free)
            freeRegs.push_back(&regs[i]);
    }
}

Reg* RegManager::allocReg() {
    if (freeRegs.empty()) return nullptr;
    auto reg = freeRegs.back();
    reg->free = false;
    freeRegs.pop_back();
    useRegs.insert(useRegs.begin(), reg);
    return reg;
}

void RegManager::freeReg() {
    auto reg = useRegs.back();
    useRegs.pop_back();
    freeRegs.push_back(reg);
    if (reg->dirty) {
        auto var = regMap[reg];
        auto variable = var->isGlobal() ? "global_" + to_string(var->getId()) :
                                                 to_string(var->getId()) + "($sp)";
        mips->emplace_back(tab + "sw " + reg->show() + ", " + variable);
        reg->dirty = false;
    }
    reg->free = true;
    reg->setVar(nullptr);
    regMap[reg] = nullptr;
}

void RegManager::setReg(Reg *reg, Variable *var) {
    auto variable = var->isGlobal() ? "global_" + to_string(var->getId()) : to_string(var->getId() << 2) + "($sp)";
    mips->emplace_back(tab + "lw " + reg->show() + ", " + variable);
    reg->setVar(var);
    regMap[reg] = var;
}

Reg *RegManager::getReg(Variable *var) {
    for (auto it = useRegs.begin(); it != useRegs.end(); it++) {
        if (regMap[*it] == var) {
            auto reg = *it;
            useRegs.erase(it);
            useRegs.insert(useRegs.begin(), *it);
            return reg;
        }
    }
    return nullptr;
}

Reg *RegManager::insertVar(Variable *var) {
    if (auto reg = getReg(var)) return reg;
    if (auto reg = allocReg()) {
        setReg(reg, var);
        return reg;
    }
    freeReg();
    auto reg = allocReg();
    setReg(reg, var);
}

