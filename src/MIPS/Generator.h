//
// Created by tjan2 on 19/11/2021.
//

#ifndef SRC_GENERATOR_H
#define SRC_GENERATOR_H


#include "../IR/Module.h"

enum MipsOp {
    addu, addiu, subu, mult, mul, divide,
    mflo, mfhi,
    beq, bne, blt, ble, bgt, bge,
    j, jal, jr,
    lw, sw, la, li,
    move, syscall
};

enum Reg {
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

class Generator {
public:
    Generator();
    explicit Generator(Module* module);

    void show();
private:
    Module* module;
    std::vector<std::string> mips;
    const std::string tab = "    ";

    int str_id;
    std::map<std::string, int> strMap;
    std::map<int, int> varMap;

    static std::string genWordTag(int id, Variable* init);
    std::string genAsciizTag(std::string s);
    static std::string genLabelTag(int id);
    static std::string genArrayTag(IrArray* arr);
    void loadVar(Variable* var, int reg);
    void loadConst(Constant* var, int reg);
    void loadAddr(Variable* base, int reg);
    void loadWord(int addr, int reg);
    void storeWord(int addr, int reg);
    void assign(Variable* var, int reg);

    void genDataSegment();
    void genTextSegment();
    void genFuncDef(IrFunc* func);
    void genBlock(IrFunc* func, BasicBlock* block);
    void genInst(IrFunc* func, Inst* inst);
    void genBinaryInst(BinaryInst* inst);
    void genAssignInst(AssignInst* inst);
    void genReturnInst(ReturnInst* inst);
    void genCallInst(CallInst* inst);
    void genDeclInst(DeclInst* inst);
    void genGetReturnInst(GetReturnInst* inst);
    void genBranchInst(BranchInst* inst);
    void genJumpInst(JumpInst* inst);
    void genNotInst(NotInst* inst);
    void genLoadAddrInst(LoadAddrInst* inst);
    void genLoadInst(LoadInst* inst);
    void genStoreInst(StoreInst* inst);
};


#endif //SRC_GENERATOR_H
