//
// Created by tjan2 on 17/11/2021.
//

#include "Instruction.h"

#include <utility>
#include "Variable.h"

Inst::Inst() {
    prev = next = nullptr;
}

std::string Inst::show() {
    return "";
}

BinaryInst::BinaryInst() : Inst() {

}

BinaryInst::BinaryInst(Variable* var, BinaryExp *exp) : Inst() {
    switch(exp->getOp().sym) {
        case PLUS: this->op = Add; break;
        case MINU: this->op = Sub; break;
        case MULT: this->op = Mul; break;
        case DIV:  this->op = Div; break;
        case MOD:  this->op = Mod; break;
        case LSS:  this->op = Slt; break;
        case LEQ:  this->op = Sle; break;
        case GRE:  this->op = Sgt; break;
        case GEQ:  this->op = Sge; break;
        case EQL:  this->op = Seq; break;
        case NEQ:  this->op = Sne; break;
        case AND:  this->op = And; break;
        case OR:   this->op = Or;  break;
        default:   /* bug */       break;
    }
    this->var = var;
    lhs = exp->getLhs()->getVar();
    rhs = exp->getRhs()->getVar();
}

BinaryInst::BinaryInst(Variable* var, BinaryOp op, Exp *exp) {
    this->op = op;
    this->var = var;
    this->lhs = new Constant(0);
    this->rhs = exp->getVar();
}

std::string BinaryInst::show() {
    std::string ops[] = { "+", "-", "*", "/", "%", "&", "|",
                          "<", "<=", ">", ">=", "==", "!=" };
    return var->show() + " = " + lhs->show() + " " + ops[op] + " " + rhs->show();
}

AssignInst::AssignInst() : Inst() {
    this->lhs = nullptr;
    this->rhs = nullptr;
}

AssignInst::AssignInst(Variable *lhs, Variable *rhs) : Inst() {
    this->lhs = lhs;
    this->rhs = rhs;
}

std::string AssignInst::show() {
    return lhs->show() + " = " + rhs->show();
}

BranchInst::BranchInst() : Inst() {

}

BranchInst::BranchInst(BranchOp op, Variable* var, int label_id) : Inst() {
    this->op = op;
    this->var = var;
    this->label_id = label_id;
}

std::string BranchInst::show() {
    std::string out[] = {"Br", "Blt", "Ble", "Bgt", "Bge", "Beq", "Bne", "Bnez", "Beqz"};
    return out[op] + " " + var->show() + ", label_" + std::to_string(label_id);
}

JumpInst::JumpInst() {
    this->label_id = 0;
}

JumpInst::JumpInst(int label_id) {
    this->label_id = label_id;
}

std::string JumpInst::show() {
    return "Jump label_" + std::to_string(label_id);
}

ReturnInst::ReturnInst() : Inst() {
    this->var = nullptr;
}

ReturnInst::ReturnInst(Variable* var) {
    this->var = var;
}

std::string ReturnInst::show() {
    if (var) return "ret " + var->show();
    return "ret";
}

CallInst::CallInst() : Inst() {

}

CallInst::CallInst(Symbol sym) {
    this->func = nullptr;
    this->sym  = std::move(sym);
}

CallInst::CallInst(Symbol sym, std::vector<Variable*>& params) {
    this->func = nullptr;
    this->sym  = std::move(sym);
    this->params = params;
}

CallInst::CallInst(IrFunc *func) {
    this->func = func;
}

CallInst::CallInst(IrFunc *func, std::vector<Variable *>& params) {
    this->func = func;
    this->params = params;
}

std::string CallInst::show() {
    if (func) {
        std::string res;
        for (auto x : params) {
            std::string push = "push " + x->show();
            res += push + "\n";
        }
        std::string call = "call " + func->getName();
        res += call;
        return res;
    } else {
        if (sym.sym == GETINTTK) {
            return "call getint";
        } else if (sym.sym == PRINTFTK) {
            std::string res;
            for (auto x : params) {
                std::string push = "push " + x->show();
                res += push + "\n" + "call printf" + "\n";
            }
            return res.substr(0, res.size() - 1);
        }
    }
    return "";
}

NotInst::NotInst() {
    this->var = nullptr;
}

NotInst::NotInst(Variable *var) {
    this->var = var;
}

std::string NotInst::show() {
    auto str = "%" + std::to_string(var->getId());
    return str + " = !" + str;
}

LoadInst::LoadInst() : Inst() {

}

std::string LoadInst::show() {
    // TODO
    return Inst::show();
}

StoreInst::StoreInst() : Inst() {

}

std::string StoreInst::show() {
    // TODO
    return Inst::show();
}

DeclInst::DeclInst() {
    this->var = nullptr;
    this->init = nullptr;
}

DeclInst::DeclInst(Variable *var) : Inst() {
    this->var = var;
    this->init = nullptr;
}

std::string DeclInst::show() {
    if (auto p = dynamic_cast<IrParam*>(var)) {
        return "para " + var->getType().getString() + " " + var->show();
    } else if (init) {
        return var->getType().getString() + " " + var->show() + " = " + init->show();
    }
    return var->getType().getString() + " " + var->show();
}

Variable *DeclInst::getVar() {
    return var;
}

void DeclInst::addInit(Variable *init) {
    this->init = init;
}

bool DeclInst::hasInit() {
    return init;
}

Variable *DeclInst::getInit() {
    return init;
}

GetReturnInst::GetReturnInst() : Inst() {

}

GetReturnInst::GetReturnInst(Variable *var) {
    this->var = var;
}

std::string GetReturnInst::show() {
    return var->show() + " = RET";
}
