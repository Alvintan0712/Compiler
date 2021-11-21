//
// Created by tjan2 on 19/11/2021.
//

#include <fstream>
#include <iostream>
#include "Generator.h"
using namespace std;

std::string mipsOps[] = {
    "addu", "addiu", "subu", "mult", "mul", "div",
    "mflo", "mfhi",
    "beq", "bne", "blt", "ble", "bgt", "bge",
    "j", "jal", "jr",
    "lw", "sw", "la", "li",
    "move", "syscall"
};

std::string mipsRegs[] = {
    "zero",
    "at",
    "v0", "v1",
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9",
    "k0", "k1",
    "gp", "sp", "fp",
    "ra"
};

Generator::Generator() {
    this->module = nullptr;
}

Generator::Generator(Module *module) {
    this->module = module;
    genDataSegment();
    genTextSegment();
}

std::string Generator::genWordTag(int id, Variable *init) {
    if (init) return "global_" + to_string(id) + ": .word " + init->show();
    return "global_" + to_string(id) + ": .word 0";
}

std::string Generator::genAsciizTag(std::string s) {
    strMap[s] = str_id;
    return "str_" + to_string(str_id++) + ": .asciiz \"" + s + "\"";
}

std::string Generator::genLabelTag(int id) {
    return "label_" + to_string(id) + ":";
}

void Generator::genDataSegment() {
    mips.emplace_back(".data");

    // genGlobals
    auto decls = module->decls;
    for (auto x : decls) {
        // TODO add genArrayTag
        string res = genWordTag(x->getVar()->getId(), x->getInit());
        mips.emplace_back(tab + res);
    }

    // genStrings
    str_id = 1;
    auto strings = module->strings;
    for (auto x : strings) {
        string res = genAsciizTag(x);
        mips.emplace_back(tab + res);
    }
}

void Generator::genTextSegment() {
    mips.emplace_back("\n.text");

    auto funcs = module->irFuncs;
    auto label = "label_" + to_string(funcs[0]->getEntryBlock()->getId());
    auto args  = to_string(funcs[0]->getVarId() - 1 << 2);
    mips.emplace_back(tab + "addiu $sp, $sp, -" + args);
    mips.emplace_back(tab + "jal " + label);
    mips.emplace_back(tab + "li $v0, 10");
    mips.emplace_back(tab + "syscall");
    for (auto x : funcs) {
        genFuncDef(x);
    }
}

void Generator::genFuncDef(IrFunc *func) {
    mips.emplace_back("# " + func->getName());
    // genBLock
    auto blk = func->getEntryBlock();
    while (blk) {
        genBlock(func, blk);
        blk = blk->next;
    }
}

void Generator::genBlock(IrFunc* func, BasicBlock *block) {
    mips.emplace_back(genLabelTag(block->getId()));
    auto inst = block->getEntryInst();
    while (inst) {
        genInst(func, inst);
        inst = inst->next;
    }
}

void Generator::loadVar(Variable *var, int reg) {
    string t = reg == 0 ? "$t0" : "$t1";
    if (var->isGlobal()) {
        string id = to_string(var->getId());
        mips.emplace_back(tab + "lw " + t + ", global_" + id);
    } else {
        string id = to_string(var->getId() - 1 << 2);
        mips.emplace_back(tab + "lw " + t + ", " + id + "($sp)");
    }
}

void Generator::genBinaryInst(BinaryInst* inst) {
    auto id  = to_string(inst->var->getId() - 1 << 2);
    auto lhs = inst->lhs;
    auto rhs = inst->rhs;

    auto left = dynamic_cast<Constant*>(lhs);
    auto right = dynamic_cast<Constant*>(rhs);

    if (left && right) {
        int x = left->getValue();
        int y = right->getValue();
        int t;
        switch(inst->op) {
            case Add: t = x + y; break;
            case Sub: t = x - y; break;
            case Mul: t = x * y; break;
            case Div: t = x / y; break;
            case Mod: t = x % y; break;
            case And: t = x && y; break;
            case Or:  t = x || y; break;
            case Slt: case Sle: case Sgt: case Sge: case Seq: case Sne: default:
                t = 0;
                break;
        }
        mips.emplace_back(tab + "addiu $t0, $0, " + to_string(t));
        // save it to stack
        mips.emplace_back(tab + "sw $t0, " + id + "($sp)");
    } else if (left) {
        // only have 0 - x
        mips.emplace_back("li $t0, " + to_string(left->getValue()));
        loadVar(rhs, 1);
        switch(inst->op) {
            case Add: mips.emplace_back(tab + "addu $t0, $t0, $t1"); break;
            case Sub: mips.emplace_back(tab + "subu $t0, $t0, $t1"); break;
            case Mul: mips.emplace_back(tab + "mul $t0, $t0, $t1"); break;
            case Div: mips.emplace_back(tab + "div $t0, $t0, $t1"); break;
            case Mod:
                mips.emplace_back(tab + "div $t0, $t1");
                mips.emplace_back(tab + "mfhi $t0");
                break;
            case And: mips.emplace_back(tab + "and $t0, $t0, $t1"); break;
            case Or:  mips.emplace_back(tab + "or $t0, $t0, $t1"); break;
            case Slt: case Sle: case Sgt: case Sge: case Seq: case Sne: default:
                break;
        }
        // save it to stack
        mips.emplace_back(tab + "sw $t0, " + id + "($sp)");
    } else if (right) {
        loadVar(lhs, 0);
        auto x = to_string(right->getValue());
        switch(inst->op) {
            case Add: mips.emplace_back(tab + "addiu $t0, $t0, " + x); break;
            case Sub: mips.emplace_back(tab + "addiu $t0, $t0, -" + x); break;
            case Mul: mips.emplace_back(tab + "mul $t0, $t0, " + x); break;
            case Div: mips.emplace_back(tab + "div $t0, $t0, " + x); break;
            case Mod:
                mips.emplace_back(tab + "div $t0, " + x);
                mips.emplace_back(tab + "mfhi $t0");
                break;
            case And: mips.emplace_back(tab + "and $t0, $t0, $t1"); break;
            case Or:  mips.emplace_back(tab + "or $t0, $t0, $t1"); break;
            case Slt: case Sle: case Sgt: case Sge: case Seq: case Sne: default:
                break;
        }
        // save it to stack
        mips.emplace_back(tab + "sw $t0, " + id + "($sp)");
    } else {
        loadVar(lhs, 0);
        loadVar(rhs, 1);
        switch(inst->op) {
            case Add: mips.emplace_back(tab + "addu $t0, $t0, $t1"); break;
            case Sub: mips.emplace_back(tab + "subu $t0, $t0, $t1"); break;
            case Mul: mips.emplace_back(tab + "mul $t0, $t0, $t1"); break;
            case Div: mips.emplace_back(tab + "div $t0, $t0, $t1"); break;
            case Mod:
                mips.emplace_back(tab + "div $t0, $t1");
                mips.emplace_back(tab + "mfhi $t0");
                break;
            case And: mips.emplace_back(tab + "and $t0, $t0, $t1"); break;
            case Or:  mips.emplace_back(tab + "or $t0, $t0, $t1"); break;
            case Slt: case Sle: case Sgt: case Sge: case Seq: case Sne: default:
                break;
        }
        // save it to stack
        mips.emplace_back(tab + "sw $t0, " + id + "($sp)");
    }
}

void Generator::genAssignInst(AssignInst *inst) {
    auto lhs = inst->lhs;
    auto rhs = inst->rhs;

    if (auto constant = dynamic_cast<Constant*>(rhs)) {
        auto val = to_string(constant->getValue());
        if (lhs->isGlobal()) {
            auto lid = to_string(lhs->getId());
            mips.emplace_back(tab + "li $t0, " + val);
            mips.emplace_back(tab + "sw $t0, global_" + lid);
        } else {
            auto lid = to_string(lhs->getId() - 1 << 2);
            mips.emplace_back(tab + "li $t0, " + val);
            mips.emplace_back(tab + "sw $t0, " + lid + "($sp)");
        }
    } else if (lhs->isGlobal() && rhs->isGlobal()) {
        auto lid = to_string(lhs->getId());
        auto rid = to_string(rhs->getId());
        mips.emplace_back(tab + "lw $t0, global_" + rid);
        mips.emplace_back(tab + "sw $t0, global_" + lid);
    } else if (lhs->isGlobal()) {
        auto lid = to_string(lhs->getId());
        auto rid = to_string(rhs->getId() - 1 << 2);
        mips.emplace_back(tab + "lw $t0, " + rid + "($sp)");
        mips.emplace_back(tab + "sw $t0, global_" + lid);
    } else if (rhs->isGlobal()) {
        auto lid = to_string(lhs->getId() - 1 << 2);
        auto rid = to_string(rhs->getId());
        mips.emplace_back(tab + "lw $t0, global_" + rid);
        mips.emplace_back(tab + "sw $t0, " + lid + "($sp)");
    } else {
        auto lid = to_string(lhs->getId() - 1 << 2);
        auto rid = to_string(rhs->getId() - 1 << 2);
        mips.emplace_back(tab + "lw $t0, " + rid + "($sp)");
        mips.emplace_back(tab + "sw $t0, " + lid + "($sp)");
    }
}

void Generator::genReturnInst(ReturnInst *inst) {
    if (inst->var) {
        auto var = inst->var;
        if (auto constant = dynamic_cast<Constant*>(var)) {
            auto x = to_string(constant->getValue());
            mips.emplace_back(tab + "addiu $v0, $0, " + x);
        } else {
            auto id = to_string(var->getId() - 1 << 2);
            mips.emplace_back(tab + "lw $v0, " + id + "($sp)");
        }
    }
    mips.emplace_back(tab + "jr $ra");
}

void Generator::genCallInst(CallInst *inst) {
    if (inst->sym.sym == GETINTTK) {
        mips.emplace_back(tab + "addiu $v0, $0, 5");
        mips.emplace_back(tab + "syscall");
    } else if (inst->sym.sym == PRINTFTK) {
        for (auto x : inst->params) {
            auto arg = dynamic_cast<IrParam*>(x);
            if (arg->constant) {
                auto constant = arg->constant;
                if (constant->type == STRCON) {
                    auto id = to_string(strMap[arg->constant->str]);
                    mips.emplace_back(tab + "la $a0, str_" + id);
                    mips.emplace_back(tab + "addiu $v0, $0, 4");
                    mips.emplace_back(tab + "syscall");
                } else {
                    auto constant = dynamic_cast<Constant*>(arg->constant);
                    auto val = to_string(constant->getValue());
                    mips.emplace_back(tab + "li $a0, " + val);
                    mips.emplace_back(tab + "addiu $v0, $0, 1");
                    mips.emplace_back(tab + "syscall");
                }
            } else {
                if (arg->isGlobal()) {
                    auto id = to_string(arg->getId());
                    mips.emplace_back(tab + "lw $a0, global_" + id);
                } else if (arg->getId() != 0) {
                    auto id = to_string(arg->getId() - 1 << 2);
                    mips.emplace_back(tab + "lw $a0, " + id + "($sp)");
                }
                mips.emplace_back(tab + "addiu $v0, $0, 1");
                mips.emplace_back(tab + "syscall");
            }
        }
    } else {
        int arg = inst->func->getVarId() - 1;
        auto args = to_string(arg << 2);
        auto label = to_string(inst->func->getEntryBlock()->getId());
        int id = 0;
        for (auto x : inst->params) {
            auto str_id = to_string(4*(id - arg - 1));
            if (auto var = dynamic_cast<Constant*>(x)) {
                auto val = to_string(var->getValue());
                mips.emplace_back(tab + "li $t0, " + val);
            } else if (x->isGlobal()) {
                auto var = to_string(x->getId());
                mips.emplace_back(tab + "lw $t0, global_" + var);
            } else {
                auto var = to_string(x->getId() - 1 << 2);
                mips.emplace_back(tab + "lw $t0, " + var + "($sp)");
            }
            mips.emplace_back(tab + "sw $t0, " + str_id + "($sp)");
            id++;
        }
        mips.emplace_back(tab + "addiu $sp, $sp, -4");
        mips.emplace_back(tab + "sw $ra, ($sp)");
        if (arg) mips.emplace_back(tab + "addiu $sp, $sp, -" + args);
        mips.emplace_back(tab + "jal label_" + label);
        if (arg) mips.emplace_back(tab + "addiu $sp, $sp, " + args);
        mips.emplace_back(tab + "lw $ra, ($sp)");
        mips.emplace_back(tab + "addiu $sp, $sp, 4");
    }
}

void Generator::genDeclInst(DeclInst *inst) {
    auto init = inst->getInit();
    if (auto param = dynamic_cast<IrParam*>(inst->getVar())) {
        // do nothing
    } else if (init == nullptr) {
        auto var = inst->getVar();
        auto id = to_string(var->getId() - 1 << 2);
        mips.emplace_back(tab + "sw $0, " + id + "($sp)");
    } else if (auto constant = dynamic_cast<Constant*>(init)) {
        auto var = inst->getVar();
        auto id = to_string(var->getId() - 1 << 2);
        auto val = to_string(constant->getValue());

        mips.emplace_back(tab + "addiu $t0, $0, " + val);
        mips.emplace_back(tab + "sw $t0, " + id + "($sp)");
    } else {
        auto lvar = inst->getVar();
        auto lid = to_string(lvar->getId() - 1 << 2);
        auto rvar = inst->getInit();
        auto rid = to_string(rvar->getId() - 1 << 2);

        mips.emplace_back(tab + "lw $t0, " + rid + "($sp)");
        mips.emplace_back(tab + "sw $t0, " + lid + "($sp)");
    }
}

void Generator::genGetReturnInst(GetReturnInst *inst) {
    auto var = inst->var;
    auto id = to_string(var->getId() - 1 << 2);
    mips.emplace_back(tab + "addu $t0, $0, $v0");
    mips.emplace_back(tab + "sw $t0, " + id + "($sp)");
}

void Generator::genInst(IrFunc* func, Inst *inst) {
    if (auto binaryInst = dynamic_cast<BinaryInst*>(inst)) {
        genBinaryInst(binaryInst);
    } else if (auto assignInst = dynamic_cast<AssignInst*>(inst)) {
        genAssignInst(assignInst);
    } else if (auto branchInst = dynamic_cast<BranchInst*>(inst)) {
        // TODO
    } else if (auto returnInst = dynamic_cast<ReturnInst*>(inst)) {
        genReturnInst(returnInst);
    } else if (auto callInst = dynamic_cast<CallInst*>(inst)) {
        genCallInst(callInst);
    } else if (auto loadInst = dynamic_cast<LoadInst*>(inst)) {
        // TODO
    } else if (auto storeInst = dynamic_cast<StoreInst*>(inst)) {
        // TODO
    } else if (auto declInst = dynamic_cast<DeclInst*>(inst)) {
        genDeclInst(declInst);
    } else if (auto getReturnInst = dynamic_cast<GetReturnInst*>(inst)) {
        genGetReturnInst(getReturnInst);
    }
}

void Generator::show() {
    ofstream f("mips.txt");
    for (const auto& x : mips) {
        f << x << endl;
        // cout << x << endl;
    }
}
