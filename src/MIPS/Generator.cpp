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
    this->str_id = 1;
    this->module = nullptr;
}

Generator::Generator(Module *module) {
    this->str_id = 1;
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

std::string Generator::genArrayTag(IrArray *arr) {
    return "global_" + to_string(arr->getId()) + ": .space " + to_string(arr->getSize()*4);
}

void Generator::genDataSegment() {
    mips.emplace_back(".data");

    // genGlobals
    auto decls = module->decls;
    for (auto x : decls) {
        auto var = x->getVar();
        if (auto arr = dynamic_cast<IrArray*>(var)) {
            auto res = genArrayTag(arr);
            mips.emplace_back(tab + res);
            if (x->hasInit()) {
                mips.emplace_back(".text");
                int n = arr->getSize();
                loadAddr(arr, 2);
                for (int i = 0; i < n; i++) {
                    auto init = x->getInits()[i];
                    if (auto constant = dynamic_cast<Constant*>(init)) {
                        loadConst(constant, 0);
                    } else {
                        loadVar(init, 0);
                    }
                    mips.emplace_back(tab + "sw $t0, ($t2)");
                    mips.emplace_back(tab + "addiu $t2, $t2, 4");
                }
                mips.emplace_back(".data");
            }
        } else {
            auto res = genWordTag(x->getVar()->getId(), x->getInit());
            mips.emplace_back(tab + res);
        }
    }

    // genStrings
    auto strings = module->strings;
    for (const auto& x : strings) {
        string res = genAsciizTag(x);
        mips.emplace_back(tab + res);
    }
}

string getLabel(int id) {
    return "label_" + to_string(id);
}

string getFuncVars(int num) {
    return to_string(num << 2);
}

void Generator::genTextSegment() {
    mips.emplace_back("\n.text");

    auto funcs = module->irFuncs;
    auto label = getLabel(funcs[0]->getEntryBlock()->getId());
    auto args  = getFuncVars(funcs[0]->getVarId());
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
    string t = "$t" + to_string(reg);
    if (var->isGlobal()) {
        string id = to_string(var->getId());
        mips.emplace_back(tab + "lw " + t + ", global_" + id);
    } else {
        string id = to_string(var->getId() << 2);
        mips.emplace_back(tab + "lw " + t + ", " + id + "($sp)");
    }
}

void Generator::loadConst(Constant *var, int reg) {
    string t = "$t" + to_string(reg);
    string val = to_string(var->getValue());
    mips.emplace_back(tab + "li " + t + ", " + val);
}

void Generator::assign(Variable* var, int reg) {
    string t = "$t" + to_string(reg);
    if (var->isGlobal()) {
        string id = to_string(var->getId());
        mips.emplace_back(tab + "sw " + t + ", global_" + id);
    } else {
        string id = to_string(var->getId() << 2);
        mips.emplace_back(tab + "sw " + t + ", " + id + "($sp)");
    }
}

void Generator::loadAddr(Variable *base, int reg) {
    string r = "$t" + to_string(reg);
    if (base->isGlobal()) {
        mips.emplace_back(tab + "la " + r + ", global_" + to_string(base->getId()));
    } else {
        mips.emplace_back(tab + "la " + r + ", " + to_string(base->getId() << 2) + "($sp)");
    }
}

void Generator::loadWord(int addr, int reg) {
    string r = "$t" + to_string(reg);
    string a = "$t" + to_string(addr);
    mips.emplace_back(tab + "lw " + r + ", (" + a + ")");
}

void Generator::storeWord(int addr, int reg) {
    string r = "$t" + to_string(reg);
    string a = "$t" + to_string(addr);
    mips.emplace_back(tab + "sw " + r + ", (" + a + ")");
}

void Generator::genBinaryInst(BinaryInst* inst) {
    auto id  = to_string(inst->var->getId() << 2);
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
            case Slt: t = x < y; break;
            case Sle: t = x <= y; break;
            case Sgt: t = x > y; break;
            case Sge: t = x >= y; break;
            case Seq: t = x == y; break;
            case Sne: t = x != y; break;
            default: t = 0; break;
        }
        mips.emplace_back(tab + "addiu $t0, $0, " + to_string(t));
        // save it to stack
        assign(inst->var, 0);
    } else if (left) {
        loadConst(left, 0);
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
            case Or:  mips.emplace_back(tab + "or  $t0, $t0, $t1"); break;
            case Slt: mips.emplace_back(tab + "slt $t0, $t0, $t1"); break;
            case Sle: mips.emplace_back(tab + "sle $t0, $t0, $t1"); break;
            case Sgt: mips.emplace_back(tab + "sgt $t0, $t0, $t1"); break;
            case Sge: mips.emplace_back(tab + "sge $t0, $t0, $t1"); break;
            case Seq: mips.emplace_back(tab + "seq $t0, $t0, $t1"); break;
            case Sne: mips.emplace_back(tab + "sne $t0, $t0, $t1"); break;
            default: break;
        }
        // save it to stack
        assign(inst->var, 0);
    } else if (right) {
        loadVar(lhs, 0);
        auto x = to_string(right->getValue());
        switch(inst->op) {
            case Add: mips.emplace_back(tab + "addiu $t0, $t0, " + x); break;
            case Sub: mips.emplace_back(tab + "addiu $t0, $t0, -" + x); break;
            case Mul: mips.emplace_back(tab + "mul $t0, $t0, " + x); break;
            case Div: mips.emplace_back(tab + "div $t0, $t0, " + x); break;
            case Mod:
                mips.emplace_back(tab + "div $t0, $t0, " + x);
                mips.emplace_back(tab + "mfhi $t0");
                break;
            case And: mips.emplace_back(tab + "and $t0, $t0, " + x); break;
            case Or:  mips.emplace_back(tab + "or  $t0, $t0, " + x); break;
            case Slt: mips.emplace_back(tab + "slti $t0, $t0, " + x); break;
            case Sle: mips.emplace_back(tab + "sle $t0, $t0, " + x); break;
            case Sgt: mips.emplace_back(tab + "sgt $t0, $t0, " + x); break;
            case Sge: mips.emplace_back(tab + "sge $t0, $t0, " + x); break;
            case Seq: mips.emplace_back(tab + "seq $t0, $t0, " + x); break;
            case Sne: mips.emplace_back(tab + "sne $t0, $t0, " + x); break;
            default: break;
        }
        // save it to stack
        assign(inst->var, 0);
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
            case Or:  mips.emplace_back(tab + "or  $t0, $t0, $t1"); break;
            case Slt: mips.emplace_back(tab + "slt $t0, $t0, $t1"); break;
            case Sle: mips.emplace_back(tab + "sle $t0, $t0, $t1"); break;
            case Sgt: mips.emplace_back(tab + "sgt $t0, $t0, $t1"); break;
            case Sge: mips.emplace_back(tab + "sge $t0, $t0, $t1"); break;
            case Seq: mips.emplace_back(tab + "seq $t0, $t0, $t1"); break;
            case Sne: mips.emplace_back(tab + "sne $t0, $t0, $t1"); break;
            default: break;
        }
        // save it to stack
        assign(inst->var, 0);
    }
}

void Generator::genAssignInst(AssignInst *inst) {
    auto lhs = inst->lhs;
    auto rhs = inst->rhs;

    if (auto constant = dynamic_cast<Constant*>(rhs)) {
        loadConst(constant, 0);
        assign(lhs, 0);
    } else {
        loadVar(rhs, 0);
        assign(lhs, 0);
    }
}

void Generator::genReturnInst(ReturnInst *inst) {
    if (inst->var) {
        auto var = inst->var;
        if (auto constant = dynamic_cast<Constant*>(var)) {
            auto x = to_string(constant->getValue());
            mips.emplace_back(tab + "addiu $v0, $0, " + x);
        } else {
            auto id = to_string(var->getId() << 2);
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
                    auto val = to_string(constant->getValue());
                    mips.emplace_back(tab + "li $a0, " + val);
                    mips.emplace_back(tab + "addiu $v0, $0, 1");
                    mips.emplace_back(tab + "syscall");
                }
            } else {
                if (arg->isGlobal()) {
                    auto id = to_string(arg->getId());
                    mips.emplace_back(tab + "lw $a0, global_" + id);
                } else {
                    auto id = to_string(arg->getId() << 2);
                    mips.emplace_back(tab + "lw $a0, " + id + "($sp)");
                }
                mips.emplace_back(tab + "addiu $v0, $0, 1");
                mips.emplace_back(tab + "syscall");
            }
        }
    } else {
        int arg = inst->func->getVarId();
        auto args = to_string(arg << 2);
        auto label = to_string(inst->func->getEntryBlock()->getId());
        int id = 0;
        for (auto x : inst->params) {
            auto str_id = to_string(4*(id - arg - 1));
            if (auto var = dynamic_cast<Constant*>(x)) {
                loadConst(var, 0);
            } else {
                loadVar(x, 0);
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
    } else if (auto arr = dynamic_cast<IrArray*>(inst->getVar())) {
        int n = arr->getSize();
        loadAddr(arr, 2);
        for (int i = 0; i < n; i++) {
            if (inst->hasInit()) {
                auto x = inst->getInits()[i];
                if (auto constant = dynamic_cast<Constant*>(x)) {
                    loadConst(constant, 0);
                } else {
                    loadVar(x, 0);
                }
                mips.emplace_back(tab + "sw $t0, ($t2)");
            } else {
                mips.emplace_back(tab + "li $t0, 0");
                mips.emplace_back(tab + "sw $t0, ($t2)");
            }
            mips.emplace_back(tab + "addiu $t2, $t2, 4");
        }
    } else if (init == nullptr) {
        auto var = inst->getVar();
        auto id = to_string(var->getId() << 2);
        mips.emplace_back(tab + "sw $0, " + id + "($sp)");
    } else if (auto constant = dynamic_cast<Constant*>(init)) {
        loadConst(constant, 0);
        assign(inst->getVar(), 0);
    } else {
        loadVar(init, 0);
        assign(inst->getVar(), 0);
    }
}

void Generator::genGetReturnInst(GetReturnInst *inst) {
    auto var = inst->var;
    auto id = to_string(var->getId() << 2);
    mips.emplace_back(tab + "addu $t0, $0, $v0");
    assign(inst->var, 0);
}

string branchOp[] = {
    "b",
    "blt",
    "ble",
    "bgt",
    "bge",
    "beq",
    "bne",
    "bnez",
    "beqz"
};

void Generator::genBranchInst(BranchInst* inst) {
    if (auto constant = dynamic_cast<Constant*>(inst->var)) {
        loadConst(constant, 0);
    } else {
        loadVar(inst->var, 0);
    }
    string id = to_string(inst->label_id);
    mips.emplace_back(tab + branchOp[inst->op] + " $t0, label_" + id);
}

void Generator::genJumpInst(JumpInst* inst) {
    string id = to_string(inst->label_id);
    mips.emplace_back(tab + "j label_" + id);
}

void Generator::genNotInst(NotInst *inst) {
    if (auto constant = dynamic_cast<Constant*>(inst->var)) {
        loadConst(constant, 0);
    } else {
        loadVar(inst->var, 0);
    }
    mips.emplace_back(tab + "seq $t0, $t0, 0");
    assign(inst->not_var, 0);
}

void Generator::genLoadAddrInst(LoadAddrInst *inst) {
    loadAddr(inst->base, 2);
    assign(inst->var, 2);
}

void Generator::genLoadInst(LoadInst *inst) {
    loadVar(inst->addr, 2);
    loadWord(2, 3);
    auto dst = inst->dst;
    if (dst->isGlobal()) {
        string id = to_string(dst->getId());
        mips.emplace_back(tab + "sw $t3, global_" + id);
    } else {
        string id = to_string(dst->getId() << 2);
        mips.emplace_back(tab + "sw $t3, " + id + "($sp)");
    }
}

void Generator::genStoreInst(StoreInst *inst) {
    loadVar(inst->val, 2);
    loadVar(inst->addr, 3);
    storeWord(3,2);
}

void Generator::genInst(IrFunc* func, Inst *inst) {
    if (auto binaryInst = dynamic_cast<BinaryInst*>(inst)) {
        genBinaryInst(binaryInst);
    } else if (auto assignInst = dynamic_cast<AssignInst*>(inst)) {
        genAssignInst(assignInst);
    } else if (auto branchInst = dynamic_cast<BranchInst*>(inst)) {
        genBranchInst(branchInst);
    } else if (auto jumpInst = dynamic_cast<JumpInst*>(inst)) {
        genJumpInst(jumpInst);
    } else if (auto returnInst = dynamic_cast<ReturnInst*>(inst)) {
        genReturnInst(returnInst);
    } else if (auto callInst = dynamic_cast<CallInst*>(inst)) {
        genCallInst(callInst);
    } else if (auto loadAddrInst = dynamic_cast<LoadAddrInst*>(inst)) {
        genLoadAddrInst(loadAddrInst);
    } else if (auto loadInst = dynamic_cast<LoadInst*>(inst)) {
        genLoadInst(loadInst);
    } else if (auto storeInst = dynamic_cast<StoreInst*>(inst)) {
        genStoreInst(storeInst);
    } else if (auto declInst = dynamic_cast<DeclInst*>(inst)) {
        genDeclInst(declInst);
    } else if (auto getReturnInst = dynamic_cast<GetReturnInst*>(inst)) {
        genGetReturnInst(getReturnInst);
    } else if (auto notInst = dynamic_cast<NotInst*>(inst)) {
        genNotInst(notInst);
    }
}

void Generator::show() {
    ofstream f("mips.txt");
    for (const auto& x : mips) {
        f << x << endl;
        // cout << x << endl;
    }
}
