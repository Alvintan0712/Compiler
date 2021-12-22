//
// Created by tjan2 on 19/11/2021.
//

#ifndef SRC_GENERATOR_H
#define SRC_GENERATOR_H


#include "../IR/Module.h"
#include "RegManager.h"

class Generator {
public:
    Generator();
    explicit Generator(Module* module, int mode, int useReg);

    void show();
private:
    Module* module;
    RegManager* regManager;
    std::vector<std::string> mips;
    const std::string tab = "    ";

    int str_id, mode, useReg;
    std::map<std::string, int> strMap;

    static std::string genWordTag(int id, Variable* init);
    std::string genAsciizTag(std::string s);
    static std::string genLabelTag(int id);
    static std::string genArrayTag(IrArray* arr);
    void loadVar(Variable* var, int reg);
    void loadConst(Constant* var, int reg);
    void loadConst(unsigned long long val, int reg);
    void loadAddr(Variable* base, int reg);
    void loadWord(int addr, int reg);
    void storeWord(int addr, int reg);
    void assign(Variable* var, int reg);

    void div_optimize(BinaryInst* inst);

    void mult_optimize(BinaryInst* inst);
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
    void genLoadHiInst(LoadHiInst* inst);

    // void loadVar(Variable* var, Reg* reg);

    void genBinaryInstUseReg(BinaryInst* inst);
    void genAssignInstUseReg(AssignInst* inst);
    void genReturnInstUseReg(ReturnInst* inst);
    void genCallInstUseReg(CallInst* inst);
    void genDeclInstUseReg(DeclInst* inst);
    void genGetReturnInstUseReg(GetReturnInst* inst);
    void genBranchInstUseReg(BranchInst* inst);
    void genNotInstUseReg(NotInst* inst);
    void genLoadAddrInstUseReg(LoadAddrInst* inst);
    void genLoadInstUseReg(LoadInst* inst);
    void genStoreInstUseReg(StoreInst* inst);
    // void genLoadHiInstUseReg(LoadHiInst* inst);

};


#endif //SRC_GENERATOR_H
