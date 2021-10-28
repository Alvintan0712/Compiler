//
// Created by tjan2 on 17/10/2021.
//

#include <iostream>
#include "Ast.h"
using namespace std;

Node::Node() {

}

BlockItem::BlockItem() {

}

void BlockItem::traverse(int lev) {

}

ProgramItem::ProgramItem() {

}

void ProgramItem::traverse(int lev) {

}

Decl::Decl() {
    Const = false;
    pointer = false;
    param = false;
    level = 0;
}

Decl::Decl(Symbol _bType, Symbol _name) {
    bType = _bType;
    name = _name;
    Const = false;
    pointer = false;
    param = false;
    level = 0;
}

void Decl::setType(Symbol _bType) {
    bType = _bType;
}

Symbol Decl::getType() {
    return bType;
}

void Decl::setConst() {
    Const = true;
}

bool Decl::isConst() {
    return Const;
}

void Decl::addDim(Exp* size) {
    dim.push_back(size);
}

int Decl::getDim() {
    return dim.size() + pointer;
}

Symbol Decl::getName() {
    return name;
}

void Decl::addInitVal(Exp* init) {
    initVal.push_back(init);
}

void Decl::setName(Symbol _name) {
    name = _name;
}

void Decl::addDim() {
    pointer = true;
}

void Decl::traverse(int lev) {
    if (!param) {
        for (int i = 0; i < lev; i++)
            cout << "    ";
    }
    if (isConst()) cout << "const ";
    cout << bType.val << " ";
    cout << name.val;
    if (pointer) cout << "[]";
    for (auto x : dim) {
        cout << "[";
        x->traverse(lev);
        cout << "]";
    }
    if (!initVal.empty()) cout << " = ";
    for (int i = 0; i < initVal.size(); i++) {
        if (i > 0) cout << ", ";
        initVal[i]->traverse(lev);
    }
    if (!param) cout << ";" << endl;
}

void Decl::addParam() {
    param = true;
}

Func::Func() {
    block = nullptr;
}

Func::Func(Symbol _returnType, Symbol _name, Block *_block) {
    returnType = _returnType;
    name = _name;
    block = _block;
}

Func::Func(Symbol _returnType, Symbol _name, Block* _block, vector<Decl*> v) {
    returnType = _returnType;
    name = _name;
    block = _block;
    FParams = v;
}

void Func::traverse(int lev) {
    cout << returnType.val << " ";
    cout << name.val << " (";
    for (int i = 0; i < FParams.size(); i++) {
        if (i > 0) cout << ", ";
        FParams[i]->traverse(lev);
    }
    cout << ")" << endl;
    block->traverse(lev);
}

BinaryExp::BinaryExp() {
    lhs = nullptr;
    rhs = nullptr;
}

void BinaryExp::setVal(Symbol _val) {
    val = _val;
}

Symbol BinaryExp::getVal() {
    return val;
}

void BinaryExp::setLhs(Exp* node) {
    lhs = node;
}

void BinaryExp::setRhs(Exp* node) {
    rhs = node;
}

void BinaryExp::traverse(int lev) {
    lhs->traverse(lev);
    cout << " " << val.val << " ";
    rhs->traverse(lev);
}

Program::Program() {

}

void Program::addItem(ProgramItem* item) {
    program_items.push_back(item);
}

std::vector<ProgramItem *> Program::getItems() {
    return program_items;
}

void Program::traverse(int lev) {
    for (auto node : program_items) {
        if (Decl* decl = dynamic_cast<Decl*>(node)) {
            decl->traverse(lev);
        } else if (Func* func = dynamic_cast<Func*>(node)) {
            func->traverse(lev);
        }
    }
}

Block::Block() {
    level = 0;
    loop = false;
}

void Block::addItem(BlockItem *item) {
    block_items.push_back(item);
}

void Block::setLevel(int lev) {
    level = lev;
}

int Block::getLevel() {
    return level;
}

void Block::addLoop() {
    loop = true;
}

bool Block::isLoop() {
    return loop;
}

void Block::traverse(int lev) {
    for (int i = 0; i < lev; i++) cout << "    ";
    cout << "{" << endl;
    lev++;
    for (auto x : block_items) {
        x->traverse(lev);
    }
    lev--;
    for (int i = 0; i < lev; i++) cout << "    ";
    cout << "}" << endl;
}

CallExp::CallExp() {

}

CallExp::CallExp(Symbol f) {
    func = f;
}

void CallExp::addParam(Exp* param) {
    rParams.push_back(param);
}

bool CallExp::isGetInt() {
    return func.sym == GETINTTK;
}

bool CallExp::isPrintf() {
    return func.sym == PRINTFTK;
}

void CallExp::traverse(int lev) {
    cout << func.val << "(";
    for (int i = 0; i < rParams.size(); i++) {
        if (i > 0) cout << ", ";
        rParams[i]->traverse(lev);
    }
    cout << ")";
}

ExpStmt::ExpStmt() {
    exp = nullptr;
}

void ExpStmt::addExp(Exp *e) {
    exp = e;
}

void ExpStmt::traverse(int lev) {
    for (int i = 0; i < lev; i++) cout << "    ";
    exp->traverse(lev);
    cout << ";" << endl;
}

UnaryExp::UnaryExp() {
    op = "";
    exp = nullptr;
}

void UnaryExp::addOp(string _op) {
    op += _op;
}

void UnaryExp::setExp(Exp *e) {
    exp = e;
}

Exp *UnaryExp::getExp() {
    return exp;
}

string UnaryExp::getOp() {
    return op;
}

void UnaryExp::addSym(Symbol name) {
    sym = name;
}

void UnaryExp::traverse(int lev) {
    cout << op;
    if (exp) exp->traverse(lev);
    else cout << sym.val;
}

LVal::LVal() {

}

LVal::LVal(Symbol sym) {
    name = sym;
}

Stmt::Stmt() {

}

void Stmt::traverse(int lev) {

}

void LVal::addDim(Exp *exp) {
    dims.push_back(exp);
}

void LVal::traverse(int lev) {
    cout << name.val;
    for (auto x : dims) {
        cout << "[";
        x->traverse(lev);
        cout << "]";
    }
}

Exp::Exp() {
    cond = false;
}

void Exp::addCond() {
    cond = true;
}

bool Exp::isCond() {
    return cond;
}

void Exp::traverse(int lev) {

}

AssignExp::AssignExp() {
    lhs = nullptr;
    rhs = nullptr;
}

AssignExp::AssignExp(LVal *l, Exp *r) {
    lhs = l;
    rhs = r;
}

void AssignExp::traverse(int lev) {
    lhs->traverse(lev);
    cout << " = ";
    rhs->traverse(lev);
}

CondStmt::CondStmt() {
    condExp = nullptr;
    ifStmt = elseStmt = nullptr;
}

CondStmt::CondStmt(Symbol name, Exp *e, Stmt *stmt) {
    sym = name;
    condExp = e;
    ifStmt = stmt;
    elseStmt = nullptr;
}

void CondStmt::addElse(Stmt *stmt) {
    elseStmt = stmt;
}

void CondStmt::traverse(int lev) {
    for (int i = 0; i < lev; i++) cout << "    ";
    cout << sym.val << " (";
    condExp->traverse(lev);
    cout << ")" << endl;
    if (Block* blk = dynamic_cast<Block*>(ifStmt)) {
        ifStmt->traverse(lev);
    } else {
        ifStmt->traverse(lev + 1);
    }
    if (elseStmt) elseStmt->traverse(lev);
}

LoopStmt::LoopStmt() {

}

LoopStmt::LoopStmt(Symbol name) {
    sym = name;
}

Symbol LoopStmt::getSym() {
    return sym;
}

void LoopStmt::traverse(int lev) {
    for (int i = 0; i < lev; i++) cout << "    ";
    cout << sym.val << ";" << endl;
}

ReturnStmt::ReturnStmt() {
    exp = nullptr;
}

ReturnStmt::ReturnStmt(Symbol name) {
    exp = nullptr;
    sym = name;
}

void ReturnStmt::addExp(Exp *e) {
    exp = e;
}

Exp *ReturnStmt::getExp() {
    return exp;
}

void ReturnStmt::traverse(int lev) {
    for (int i = 0; i < lev; i++) cout << "    ";
    cout << "return";
    if (exp) {
        cout << " ";
        exp->traverse(lev);
        cout << ";" << endl;
    } else {
        cout << ";" << endl;
    }
}

FormatString::FormatString() {

}

FormatString::FormatString(Symbol str) {
    sym = str;
}

Symbol FormatString::getSym() {
    return sym;
}

void FormatString::traverse(int lev) {
    cout << sym.val;
}

Ast::Ast() {
    program = nullptr;
    errorHandling = new ErrorHandling();
}

Ast::Ast(Program* p, ErrorHandling* err) {
    program = p;
    errorHandling = err;
}

void Ast::traverse() {
    program->traverse(0);
}
