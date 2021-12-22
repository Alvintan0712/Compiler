//
// Created by tjan2 on 19/11/2021.
//

#include <fstream>
#include <iostream>
#include <cmath>
#include "Generator.h"
using namespace std;
#define N 32
#define ll long long
#define ull unsigned long long

Generator::Generator() {
    this->mode = 0;
    this->str_id = 1;
    this->module = nullptr;
}

Generator::Generator(Module *module, int mode, int useReg) {
    this->str_id = 1;
    this->module = module;
    this->regManager = new RegManager(&mips);

    this->mode = mode;
    this->useReg = useReg;

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

void Generator::loadConst(unsigned long long val, int reg) {
    string t = "$t" + to_string(reg);
    string value = to_string(val);
    mips.emplace_back(tab + "li " + t + ", " + value);
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

ull pow2(int x) {
    return 1LLU << x;
}

tuple<ull, int, int> choose_multiplier(unsigned int d) {
    const int prec = N - 1;
    int l = ceil(log2(d));
    int sh_post = l;

    ull m_low  = floor(pow2(N + l) / d);
    ull m_high = floor((pow2(N + l) + pow2(N + l - prec)) / d);
    while (floor(m_low / 2) < floor(m_high / 2) && sh_post > 0) {
        m_low = floor(m_low / 2);
        m_high = floor(m_high / 2);
        sh_post = sh_post - 1;
    }

    return {m_high, sh_post, l};
}

int getPowerOf2(ull x) {
    if (x && ceil(log2(x)) == floor(log2(x))) return ceil(log2(x));
    return 0;
}

void Generator::div_optimize(BinaryInst *inst) {
    auto n = inst->lhs;
    auto rhs = dynamic_cast<Constant*>(inst->rhs);
    auto q = inst->var;
    int d = rhs->getValue();
    auto t = choose_multiplier(d);
    ull m = get<0>(t);
    int sh_post = get<1>(t), l = get<2>(t);

    if (abs(d) == 1) {
        loadVar(n, 1);
    } else if (abs(d) == pow2(l)) {
        // n was load in $t0
        mips.emplace_back(tab + "sra $t1, $t0, " + to_string(l - 1));
        mips.emplace_back(tab + "srl $t1, $t1, " + to_string(N - l));
        mips.emplace_back(tab + "addu $t1, $t0, $t1");
        if (l) mips.emplace_back(tab + "sra $t1, $t1, " + to_string(l));
    } else if (m < pow2(N - 1)) {
        // n was load in $t0
        if (int x = getPowerOf2(m)) mips.emplace_back(tab + "sll $t1, $t0, " = to_string(x));
        else mips.emplace_back(tab + "mul $t1, $t0, " + to_string(m));
        mips.emplace_back(tab + "mfhi $t1");
        if (sh_post) mips.emplace_back(tab + "sra $t1, $t1, " + to_string(sh_post));
        mips.emplace_back(tab + "sra $t2, $t0, 31");
        mips.emplace_back(tab + "subu $t1, $t1, $t2");
    } else {
        // n was load in $t0
        loadConst(m, 1);
        mips.emplace_back(tab + "subu $t1, $t1, " + to_string(pow2(N)));
        mips.emplace_back(tab + "mul $t1, $t0, $t1");
        mips.emplace_back(tab + "mfhi $t1");
        mips.emplace_back(tab + "addu $t1, $t0, $t1");
        if (sh_post) mips.emplace_back(tab + "sra $t1, $t1, " + to_string(sh_post));
        mips.emplace_back(tab + "sra $t2, $t0, 31");
        mips.emplace_back(tab + "subu $t1, $t1, $t2");
    }

    if (inst->op == Mod) {
        if (int x = getPowerOf2(rhs->getValue()))
            mips.emplace_back(tab + "sll $t1, $t1, " + to_string(x));
        else
            mips.emplace_back(tab + "mul $t1, $t1, " + to_string(rhs->getValue()));
        mips.emplace_back(tab + "subu $t1, $t0, $t1");
    } else if (d < 0) {
        mips.emplace_back(tab + "subu $t1, $0, $t1");
    }
    assign(q, 1);
}

void Generator::mult_optimize(BinaryInst *inst) {
    auto lhs = inst->lhs;
    auto rhs = inst->rhs;

    if (auto left = dynamic_cast<Constant*>(lhs)) {
        // left in $t0, right in $t1
        int d = left->getValue();
        if (int x = getPowerOf2(abs(d))) {
            mips.emplace_back(tab + "sll $t0, $t1, " + to_string(x));
            if (d < 0) mips.emplace_back(tab + "subu $t0, $0, $t0");
        } else {
            mips.emplace_back(tab + "mul $t0, $t0, $t1");
        }
    } else if (auto right = dynamic_cast<Constant*>(rhs)) {
        // left in $t0
        int d = right->getValue();
        if (int x = getPowerOf2(abs(d))) {
            mips.emplace_back(tab + "sll $t0, $t0, " + to_string(x));
            if (d < 0) mips.emplace_back(tab + "subu $t0, $0, $t0");
        } else {
            mips.emplace_back(tab + "mul $t0, $t0, " + to_string(d));
        }
    }
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
            case Sll: t = x << y; break;
            case Srl: t = (unsigned int) x >> y; break;
            case Sra: t = x >> y; break;
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
            case Mul:
                if (mode >= 2) mult_optimize(inst);
                else mips.emplace_back(tab + "mul $t0, $t0, $t1");
                break;
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
            case Sll: mips.emplace_back(tab + "sll $t0, $t0, $t1"); break;
            case Srl: mips.emplace_back(tab + "srl $t0, $t0, $t1"); break;
            case Sra: mips.emplace_back(tab + "sra $t0, $t0, $t1"); break;
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
            case Mul:
                if (mode >= 2) mult_optimize(inst);
                else mips.emplace_back(tab + "mul $t0, $t0, " + x);
                break;
            case Div:
                if (mode >= 1) {
                    div_optimize(inst);
                    return;
                }
                else mips.emplace_back(tab + "div $t0, $t0, " + x);
                break;
            case Mod:
                if (mode >= 1) {
                    div_optimize(inst);
                    return;
                } else {
                    mips.emplace_back(tab + "div $t0, $t0, " + x);
                    mips.emplace_back(tab + "mfhi $t0");
                }
                break;
            case And: mips.emplace_back(tab + "and $t0, $t0, " + x); break;
            case Or:  mips.emplace_back(tab + "or  $t0, $t0, " + x); break;
            case Slt: mips.emplace_back(tab + "slti $t0, $t0, " + x); break;
            case Sle: mips.emplace_back(tab + "sle $t0, $t0, " + x); break;
            case Sgt: mips.emplace_back(tab + "sgt $t0, $t0, " + x); break;
            case Sge: mips.emplace_back(tab + "sge $t0, $t0, " + x); break;
            case Seq: mips.emplace_back(tab + "seq $t0, $t0, " + x); break;
            case Sne: mips.emplace_back(tab + "sne $t0, $t0, " + x); break;
            case Sll: mips.emplace_back(tab + "sll $t0, $t0, " + x); break;
            case Srl: mips.emplace_back(tab + "srl $t0, $t0, " + x); break;
            case Sra: mips.emplace_back(tab + "sra $t0, $t0, " + x); break;
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
            case Sll: mips.emplace_back(tab + "sll $t0, $t0, $t1"); break;
            case Srl: mips.emplace_back(tab + "srl $t0, $t0, $t1"); break;
            case Sra: mips.emplace_back(tab + "sra $t0, $t0, $t1"); break;
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
                // mips.emplace_back(tab + "li $t0, 0");
                // mips.emplace_back(tab + "sw $t0, ($t2)");
            }
            mips.emplace_back(tab + "addiu $t2, $t2, 4");
        }
    } else if (init == nullptr) {
        // auto var = inst->getVar();
        // auto id = to_string(var->getId() << 2);
        // mips.emplace_back(tab + "sw $0, " + id + "($sp)");
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

void Generator::genLoadHiInst(LoadHiInst *inst) {
    mips.emplace_back(tab + "mfhi $t0");
    assign(inst->var, 0);
}

void Generator::genBinaryInstUseReg(BinaryInst *inst) {
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
            case Sll: t = x << y; break;
            case Srl: t = (unsigned int) x >> y; break;
            case Sra: t = x >> y; break;
            default: t = 0; break;
        }
        auto reg = regManager->insertVar(inst->var);
        mips.emplace_back(tab + "li " + reg->show() + to_string(t));
        reg->setDirty();
    } else if (left) {
        Reg* reg_var = regManager->insertVar(inst->var);
        Reg *reg_lhs, *reg_rhs;
        switch(inst->op) {
            case Add:
                reg_rhs = regManager->insertVar(rhs);
                mips.emplace_back(tab + "addu " + reg_var->show() + ", " + reg_rhs->show() + ", " + left->show());
                break;
            case Sub:
                reg_lhs = regManager->insertVar(left);
                reg_rhs = regManager->insertVar(rhs);
                mips.emplace_back(tab + "subu " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Mul:
                if (mode >= 2) mult_optimize(inst);
                else {
                    reg_rhs = regManager->insertVar(rhs);
                    mips.emplace_back(tab + "mul " + reg_var->show() + ", " + reg_rhs->show() + ", " + left->show());
                }
                break;
            case Div:
                reg_lhs = regManager->insertVar(left);
                reg_rhs = regManager->insertVar(rhs);
                mips.emplace_back(tab + "div " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Mod:
                reg_lhs = regManager->insertVar(left);
                reg_rhs = regManager->insertVar(rhs);
                mips.emplace_back(tab + "div " + reg_lhs->show() + ", " + reg_rhs->show());
                mips.emplace_back(tab + "mfhi " + reg_var->show());
                break;
            // TODO
            case And: mips.emplace_back(tab + "and $t0, $t0, $t1"); break;
            // TODO
            case Or:  mips.emplace_back(tab + "or  $t0, $t0, $t1"); break;
            case Slt:
                reg_rhs = regManager->insertVar(rhs);
                mips.emplace_back(tab + "sgt " + reg_var->show() + ", " + reg_rhs->show() + ", " + left->show());
                break;
            case Sle:
                reg_rhs = regManager->insertVar(rhs);
                mips.emplace_back(tab + "sge " + reg_var->show() + ", " + reg_rhs->show() + ", " + left->show());
                break;
            case Sgt:
                reg_rhs = regManager->insertVar(rhs);
                mips.emplace_back(tab + "slti " + reg_var->show() + ", " + reg_rhs->show() + ", " + left->show());
                break;
            case Sge:
                reg_rhs = regManager->insertVar(rhs);
                mips.emplace_back(tab + "sle " + reg_var->show() + ", " + reg_rhs->show() + ", " + left->show());
                break;
            case Seq:
                reg_rhs = regManager->insertVar(rhs);
                mips.emplace_back(tab + "seq " + reg_var->show() + ", " + reg_rhs->show() + ", " + left->show());
                break;
            case Sne:
                reg_rhs = regManager->insertVar(rhs);
                mips.emplace_back(tab + "sne " + reg_var->show() + ", " + reg_rhs->show() + ", " + left->show());
                break;
            // TODO
            case Sll: mips.emplace_back(tab + "sll $t0, $t0, $t1"); break;
            // TODO
            case Srl: mips.emplace_back(tab + "srl $t0, $t0, $t1"); break;
            // TODO
            case Sra: mips.emplace_back(tab + "sra $t0, $t0, $t1"); break;
            default: break;
        }
        reg_var->setDirty();
    } else if (right) {
        Reg* reg_var = regManager->insertVar(inst->var);
        Reg *reg_lhs, *reg_rhs;
        auto x = to_string(right->getValue());
        switch(inst->op) {
            case Add:
                reg_lhs = regManager->insertVar(lhs);
                mips.emplace_back(tab + "addiu " + reg_var->show() + ", " + reg_lhs->show() + ", " + x);
                break;
            case Sub:
                reg_lhs = regManager->insertVar(lhs);
                mips.emplace_back(tab + "addiu " + reg_var->show() + ", " + reg_lhs->show() + ", -" + x);
                break;
            case Mul:
                if (mode >= 2) mult_optimize(inst);
                else {
                    reg_lhs = regManager->insertVar(lhs);
                    mips.emplace_back(tab + "mul " + reg_var->show() + ", " + reg_lhs->show() + ", " + x);
                }
                break;
            case Div:
                if (mode >= 1) {
                    div_optimize(inst);
                    return;
                }
                else {
                    reg_lhs = regManager->insertVar(lhs);
                    mips.emplace_back(tab + "div " + reg_var->show() + ", " + reg_lhs->show() + ", " + x);
                }
                break;
            case Mod:
                if (mode >= 1) {
                    div_optimize(inst);
                    return;
                } else {
                    reg_lhs = regManager->insertVar(lhs);
                    mips.emplace_back(tab + "div " + reg_var->show() + ", " + reg_lhs->show() + ", " + x);
                    mips.emplace_back(tab + "mfhi " + reg_var->show());
                }
                break;
            // TODO
            case And: mips.emplace_back(tab + "and $t0, $t0, " + x); break;
            // TODO
            case Or:  mips.emplace_back(tab + "or  $t0, $t0, " + x); break;
            case Slt:
                reg_lhs = regManager->insertVar(lhs);
                mips.emplace_back(tab + "slti " + reg_var->show() + ", " + reg_lhs->show() + ", " + x);
                break;
            case Sle:
                reg_lhs = regManager->insertVar(lhs);
                mips.emplace_back(tab + "sle " + reg_var->show() + ", " + reg_lhs->show() + ", " + x);
                break;
            case Sgt:
                reg_lhs = regManager->insertVar(lhs);
                mips.emplace_back(tab + "sgt " + reg_var->show() + ", " + reg_lhs->show() + ", " + x);
                break;
            case Sge:
                reg_lhs = regManager->insertVar(lhs);
                mips.emplace_back(tab + "sge " + reg_var->show() + ", " + reg_lhs->show() + ", " + x);
                break;
            case Seq:
                reg_lhs = regManager->insertVar(lhs);
                mips.emplace_back(tab + "seq " + reg_var->show() + ", " + reg_lhs->show() + ", " + x);
                break;
            case Sne:
                reg_lhs = regManager->insertVar(lhs);
                mips.emplace_back(tab + "sne " + reg_var->show() + ", " + reg_lhs->show() + ", " + x);
                break;
            // TODO
            case Sll: mips.emplace_back(tab + "sll $t0, $t0, " + x); break;
            // TODO
            case Srl: mips.emplace_back(tab + "srl $t0, $t0, " + x); break;
            // TODO
            case Sra: mips.emplace_back(tab + "sra $t0, $t0, " + x); break;
            default: break;
        }
        reg_var->setDirty();
    } else {
        Reg* reg_var = regManager->insertVar(inst->var);
        Reg *reg_lhs = regManager->insertVar(inst->lhs), *reg_rhs = regManager->insertVar(inst->rhs);
        switch(inst->op) {
            case Add:
                mips.emplace_back(tab + "addu " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Sub:
                mips.emplace_back(tab + "subu " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Mul:
                mips.emplace_back(tab + "mul " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Div:
                mips.emplace_back(tab + "div " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Mod:
                mips.emplace_back(tab + "div " + reg_lhs->show() + ", " + reg_rhs->show());
                mips.emplace_back(tab + "mfhi " + reg_lhs->show());
                break;
            // TODO
            case And: mips.emplace_back(tab + "and $t0, $t0, $t1"); break;
            // TODO
            case Or:  mips.emplace_back(tab + "or  $t0, $t0, $t1"); break;
            case Slt:
                mips.emplace_back(tab + "slt " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Sle:
                mips.emplace_back(tab + "sle " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Sgt:
                mips.emplace_back(tab + "sgt " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Sge:
                mips.emplace_back(tab + "sge " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Seq:
                mips.emplace_back(tab + "seq " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            case Sne:
                mips.emplace_back(tab + "sne " + reg_var->show() + ", " + reg_lhs->show() + ", " + reg_rhs->show());
                break;
            // TODO
            case Sll: mips.emplace_back(tab + "sll $t0, $t0, $t1"); break;
            // TODO
            case Srl: mips.emplace_back(tab + "srl $t0, $t0, $t1"); break;
            // TODO
            case Sra: mips.emplace_back(tab + "sra $t0, $t0, $t1"); break;
            default: break;
        }
        reg_var->setDirty();
    }
}

void Generator::genAssignInstUseReg(AssignInst *inst) {
    auto lhs = inst->lhs;
    auto rhs = inst->rhs;

    if (auto constant = dynamic_cast<Constant*>(rhs)) {
        auto reg_lhs = regManager->insertVar(lhs);
        mips.emplace_back(tab + "li " + reg_lhs->show() + ", " + constant->show());
        reg_lhs->setDirty();
    } else {
        auto reg_lhs = regManager->insertVar(lhs);
        auto reg_rhs = regManager->insertVar(rhs);
        mips.emplace_back(tab + "move " + reg_lhs->show() + ", " + reg_rhs->show());
        reg_lhs->setDirty();
    }
}

void Generator::genReturnInstUseReg(ReturnInst *inst) {
    if (inst->var) {
        auto var = inst->var;
        if (auto constant = dynamic_cast<Constant*>(var)) {
            mips.emplace_back(tab + "li $v0, " + constant->show());
        } else {
            if (auto reg = regManager->getReg(var)) {
                mips.emplace_back(tab + "move $v0, " + reg->show());
            } else {
                auto id = to_string(var->getId() << 2);
                mips.emplace_back(tab + "lw $v0, " + id + "($sp)");
            }
        }
    }
    mips.emplace_back(tab + "jr $ra");
}

void Generator::genCallInstUseReg(CallInst *inst) {
    if (inst->sym.sym == GETINTTK) {
        mips.emplace_back(tab + "li $v0, 5");
        mips.emplace_back(tab + "syscall");
    } else if (inst->sym.sym == PRINTFTK) {
        for (auto x : inst->params) {
            auto arg = dynamic_cast<IrParam*>(x);
            if (arg->constant) {
                auto constant = arg->constant;
                if (constant->type == STRCON) {
                    auto id = to_string(strMap[arg->constant->str]);
                    mips.emplace_back(tab + "la $a0, str_" + id);
                    mips.emplace_back(tab + "li $v0, 4");
                    mips.emplace_back(tab + "syscall");
                } else {
                    auto val = to_string(constant->getValue());
                    mips.emplace_back(tab + "li $a0, " + val);
                    mips.emplace_back(tab + "li $v0, 1");
                    mips.emplace_back(tab + "syscall");
                }
            } else {
                if (arg->isGlobal()) {
                    if (auto reg = regManager->getReg(arg->var)) {
                        mips.emplace_back(tab + "move $a0, " + reg->show());
                    } else {
                        auto id = to_string(arg->getId());
                        mips.emplace_back(tab + "lw $a0, global_" + id);
                    }
                } else {
                    if (auto reg = regManager->getReg(arg->var)) {
                        cout << "in reg" << endl;
                        mips.emplace_back(tab + "move $a0, " + reg->show());
                    } else {
                        cout << "not in reg" << endl;
                        auto id = to_string(arg->getId() << 2);
                        mips.emplace_back(tab + "lw $a0, " + id + "($sp)");
                    }
                }
                mips.emplace_back(tab + "li $v0, 1");
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

void Generator::genDeclInstUseReg(DeclInst *inst) {
    auto init = inst->getInit();
    if (auto param = dynamic_cast<IrParam*>(inst->getVar())) {
        // do nothing
    } else if (auto arr = dynamic_cast<IrArray*>(inst->getVar())) {
        int n = arr->getSize();
        if (arr->isGlobal()) mips.emplace_back(tab + "la $v0, global_" + to_string(arr->getId()));
        else mips.emplace_back(tab + "la $v0, " + to_string(arr->getId() << 2) + "($sp)");
        for (int i = 0; i < n; i++) {
            if (inst->hasInit()) {
                auto x = inst->getInits()[i];
                if (auto constant = dynamic_cast<Constant*>(x)) {
                    mips.emplace_back(tab + "li $a0, " + constant->show());
                } else {
                    if (x->isGlobal()) mips.emplace_back(tab + "lw $a0, global_" + to_string(x->getId()));
                    else mips.emplace_back(tab + "lw $a0, " + to_string(x->getId() << 2) + "($sp)");
                }
                mips.emplace_back(tab + "sw $a0, ($v0)");
            } else {
                // mips.emplace_back(tab + "sw $0, ($v0)");
            }
            mips.emplace_back(tab + "addiu $v0, $v0, 4");
        }
    } else if (init == nullptr) {
        // auto var = inst->getVar();
        // auto id = to_string(var->getId() << 2);
        // mips.emplace_back(tab + "sw $0, " + id + "($sp)");
    } else if (auto constant = dynamic_cast<Constant*>(init)) {
        if (constant->getValue()) {
            mips.emplace_back(tab + "li $v0, " + constant->show());
            mips.emplace_back(tab + "sw $v0, " + to_string(inst->getVar()->getId()));
        }
    } else {
        auto reg = regManager->insertVar(init);
        if (inst->getVar()->isGlobal()) {
            mips.emplace_back(tab + "sw " + reg->show() + ", global_" + to_string(inst->getVar()->getId()));
        } else {
            mips.emplace_back(tab + "sw " + reg->show() + ", " + to_string(inst->getVar()->getId() << 2) + "($sp)");
        }
    }
}

void Generator::genGetReturnInstUseReg(GetReturnInst *inst) {
    auto var = inst->var;
    auto reg = regManager->insertVar(var);
    mips.emplace_back(tab + "move " + reg->show() + ", $v0");
}

void Generator::genBranchInstUseReg(BranchInst *inst) {
    auto reg = regManager->insertVar(inst->var);
    string id = to_string(inst->label_id);
    mips.emplace_back(tab + branchOp[inst->op] + " " + reg->show() + ", label_" + id);
}

void Generator::genNotInstUseReg(NotInst *inst) {
    auto reg = regManager->insertVar(inst->var);
    auto not_reg = regManager->insertVar(inst->not_var);
    mips.emplace_back(tab + "seq " + not_reg->show() + ", " + reg->show() + ", 0");
    // not_reg->setDirty();
}

void Generator::genLoadAddrInstUseReg(LoadAddrInst *inst) {
    auto reg = regManager->insertVar(inst->var);
    if (inst->base->isGlobal()) {
        mips.emplace_back(tab + "la " + reg->show() + ", global_" + to_string(inst->base->getId()));
    } else {
        mips.emplace_back(tab + "la " + reg->show() + ", " + to_string(inst->base->getId() << 2) + "($sp)");
    }
    reg->setDirty();
}

void Generator::genLoadInstUseReg(LoadInst *inst) {
    auto addr_reg = regManager->insertVar(inst->addr);
    auto reg = regManager->insertVar(inst->dst);
    mips.emplace_back(tab + "lw " + reg->show() + ", (" + addr_reg->show() + ")");
    reg->setDirty();
}

void Generator::genStoreInstUseReg(StoreInst *inst) {
    auto addr_reg = regManager->insertVar(inst->addr);
    auto reg = regManager->insertVar(inst->val);
    mips.emplace_back(tab + "sw " + reg->show() + ", (" + addr_reg->show() + ")");
}

void Generator::genInst(IrFunc* func, Inst *inst) {
    if (auto binaryInst = dynamic_cast<BinaryInst*>(inst)) {
        useReg ? genBinaryInstUseReg(binaryInst) : genBinaryInst(binaryInst);
    } else if (auto assignInst = dynamic_cast<AssignInst*>(inst)) {
        useReg ? genAssignInstUseReg(assignInst) : genAssignInst(assignInst);
    } else if (auto branchInst = dynamic_cast<BranchInst*>(inst)) {
        useReg ? genBranchInstUseReg(branchInst) : genBranchInst(branchInst);
    } else if (auto jumpInst = dynamic_cast<JumpInst*>(inst)) {
        genJumpInst(jumpInst);
    } else if (auto returnInst = dynamic_cast<ReturnInst*>(inst)) {
        useReg ? genReturnInstUseReg(returnInst) : genReturnInst(returnInst);
    } else if (auto callInst = dynamic_cast<CallInst*>(inst)) {
        useReg ? genCallInstUseReg(callInst) : genCallInst(callInst);
    } else if (auto loadAddrInst = dynamic_cast<LoadAddrInst*>(inst)) {
        useReg ? genLoadAddrInstUseReg(loadAddrInst) : genLoadAddrInst(loadAddrInst);
    } else if (auto loadInst = dynamic_cast<LoadInst*>(inst)) {
        useReg ? genLoadInstUseReg(loadInst) : genLoadInst(loadInst);
    } else if (auto storeInst = dynamic_cast<StoreInst*>(inst)) {
        useReg ? genStoreInstUseReg(storeInst) : genStoreInst(storeInst);
    } else if (auto declInst = dynamic_cast<DeclInst*>(inst)) {
        useReg ? genDeclInstUseReg(declInst) : genDeclInst(declInst);
    } else if (auto getReturnInst = dynamic_cast<GetReturnInst*>(inst)) {
        useReg ? genGetReturnInstUseReg(getReturnInst) : genGetReturnInst(getReturnInst);
    } else if (auto notInst = dynamic_cast<NotInst*>(inst)) {
        useReg ? genNotInstUseReg(notInst) : genNotInst(notInst);
    } else if (auto loadHiInst = dynamic_cast<LoadHiInst*>(inst)) {
        // useless
        genLoadHiInst(loadHiInst);
    }
}

void Generator::show() {
    ofstream f("mips.txt");
    for (const auto& x : mips) {
        f << x << endl;
        // cout << x << endl;
    }
}
