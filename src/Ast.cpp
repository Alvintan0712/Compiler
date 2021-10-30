//
// Created by tjan2 on 17/10/2021.
//

#include <iostream>
#include "Ast.h"
#include "Table.h"

using namespace std;

Table table;
ErrorHandling* errorHandling;
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
    bType = Type(_bType);
    name = _name;
    Const = false;
    pointer = false;
    param = false;
    level = 0;
}

void Decl::setType(Symbol _bType) {
    bType = Type(_bType);
}

Type Decl::getType() {
    return bType;
}

void Decl::setConst() {
    Const = true;
}

bool Decl::isConst() {
    return Const;
}

void Decl::addDim() {
    bType.addDim(0);
    pointer = true;
}

void Decl::addDim(Exp* size) {
    bType.addDim(size->evalInt());
}

int Decl::getDim() {
    return bType.getDims().size() + pointer;
}

Symbol Decl::getName() {
    return name;
}

void Decl::addInitVal(Exp* init) {
    initVal.push_back(init);
}

std::vector<int> Decl::getInitVal() {
    vector<int> v;
    for (auto x : initVal) {
        v.push_back(x->evalInt());
    }
    return v;
}

void Decl::setName(Symbol _name) {
    name = _name;
}

void Decl::traverse(int lev) {
    table.pushDecl(this);
    if (!param) {
        for (int i = 0; i < lev; i++)
            cout << "    ";
    }
    if (isConst()) cout << "const ";
    cout << bType.getType().val << " ";
    cout << name.val;
    if (pointer) cout << "[]";
    vector<int> dims = bType.getDims();
    for (auto x : dims) {
        cout << "[";
        cout << x;
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
    returnType = Type(_returnType);
    name = _name;
    block = _block;
}

Func::Func(Symbol _returnType, Symbol _name, Block* _block, vector<Decl*> v) {
    returnType = Type(_returnType);
    name = _name;
    block = _block;
    FParams = v;
}

Symbol Func::getName() {
    return name;
}

std::vector<Decl *> Func::getParams() {
    return FParams;
}

Type Func::getType() {
    return returnType;
}

void Func::checkReturn() {
    ReturnStmt* rs = block->evalReturn();
    if (returnType.getType().sym != VOIDTK && (rs == nullptr || rs->getExp() == nullptr)) {
        errorHandling->funcNeedReturn(block->getRBrace());
    } else if (returnType.getType().sym == VOIDTK &&
               rs && rs->getExp() &&
               rs->getExp()->evalType().getType().sym != VOIDTK) {
        errorHandling->funcNoNeedReturn(rs->getSymbol());
    }
}

void Func::checkLoop() {
    block->checkLoop();
}

void Func::traverse(int lev) {
    checkLoop();
    table.pushFunc(this);
    table.pushBlock();
    cout << returnType.getType().val << " ";
    cout << name.val << " (";
    for (int i = 0; i < FParams.size(); i++) {
        if (i > 0) cout << ", ";
        FParams[i]->traverse(lev);
    }
    cout << ")" << endl;
    checkReturn();
    block->traverse(lev);
    table.popBlock();
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

Type BinaryExp::evalType() {
    return Exp::evalType();
}

int BinaryExp::evalInt() {
    if (val.sym == PLUS) {
        return lhs->evalInt() + rhs->evalInt();
    } else if (val.sym == MINU) {
        return lhs->evalInt() - rhs->evalInt();
    } else if (val.sym == MULT) {
        return lhs->evalInt() * rhs->evalInt();
    } else if (val.sym == DIV) {
        return lhs->evalInt() / rhs->evalInt();
    } else if (val.sym == MOD) {
        return lhs->evalInt() % rhs->evalInt();
    } else if (val.sym == AND) {
        return lhs->evalInt() && rhs->evalInt();
    } else if (val.sym == OR) {
        return lhs->evalInt() || rhs->evalInt();
    } else if (val.sym == LSS) {
        return lhs->evalInt() < rhs->evalInt();
    } else if (val.sym == LEQ) {
        return lhs->evalInt() <= rhs->evalInt();
    } else if (val.sym == GRE) {
        return lhs->evalInt() > rhs->evalInt();
    } else if (val.sym == GEQ) {
        return lhs->evalInt() >= rhs->evalInt();
    } else if (val.sym == EQL) {
        return lhs->evalInt() == rhs->evalInt();
    } else if (val.sym == NEQ) {
        return lhs->evalInt() != rhs->evalInt();
    }
    return 0;
}

Program::Program() {

}

void Program::addError(ErrorHandling* error) {
    err = error;
}

void Program::addItem(ProgramItem* item) {
    program_items.push_back(item);
}

std::vector<ProgramItem *> Program::getItems() {
    return program_items;
}

void Program::traverse(int lev) {
    table.pushBlock();
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

void Block::setLBrace(Symbol sym) {
    lBrace = sym;
}

Symbol Block::getLBrace() {
    return lBrace;
}

void Block::setRBrace(Symbol sym) {
    rBrace = sym;
}

Symbol Block::getRBrace() {
    return rBrace;
}

ReturnStmt* Block::evalReturn() {
    if (block_items.empty()) return nullptr;
    for (auto it = block_items.rbegin(); it != block_items.rend(); it++) {
        if (auto* r = dynamic_cast<ReturnStmt*>(*it))
            return r;
    }
    return nullptr;
}

vector<LoopStmt*> Block::evalLoop() {
    vector<LoopStmt*> v;
    for (auto x : block_items) {
        if (auto* b = dynamic_cast<Block*>(x)) {
            auto ret = b->evalLoop();
            v.insert(v.end(), ret.begin(), ret.end());
        } else if (auto* r = dynamic_cast<LoopStmt*>(x)) {
            v.push_back(r);
        }
    }
    return v;
}

void Block::checkLoop() {
    for (auto item : block_items) {
        if (auto* w = dynamic_cast<CondStmt*>(item)) {
            if (w->getSym().sym == IFTK) {
                if (auto* blk = dynamic_cast<Block*>(w->getIfStmt())) {
                    blk->checkLoop();
                } else if (auto* loop = dynamic_cast<LoopStmt*>(w->getIfStmt())) {
                    errorHandling->loopError(loop->getSym());
                }
                if (w->getElseStmt()) {
                    if (auto* blk = dynamic_cast<Block*>(w->getIfStmt())) {
                        blk->checkLoop();
                    } else if (auto* loop = dynamic_cast<LoopStmt*>(w->getIfStmt())) {
                        errorHandling->loopError(loop->getSym());
                    }
                }
            }
        } else if (auto* blk = dynamic_cast<Block*>(item)) {
            blk->checkLoop();
        } else if (auto* loop = dynamic_cast<LoopStmt*>(item)) {
            errorHandling->loopError(loop->getSym());
        }
    }
}

void Block::traverse(int lev) {
    for (int i = 0; i < lev; i++) cout << "    ";
    cout << "{" << endl;
    lev++;
    for (auto x : block_items) {
        if (auto* blk = dynamic_cast<Block*>(x)) {
            table.pushBlock();
            blk->traverse(lev);
            table.popBlock();
        } else {
            x->traverse(lev);
        }
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

std::vector<Exp *> CallExp::getParams() {
    return rParams;
}

bool CallExp::isGetInt() {
    return func.sym == GETINTTK;
}

bool CallExp::isPrintf() {
    return func.sym == PRINTFTK;
}

Symbol CallExp::getFunc() {
    return func;
}

void CallExp::checkPrintf() {
    if (isPrintf()) {
        int cnt = 0;
        if (auto* f = dynamic_cast<FormatString*>(rParams[0])) {
            string s = f->getSym().val;
            for (int i = 0; i < s.size(); i++) {
                if (s[i] == '%' && i + 1 < s.size() && s[i + 1] == 'd')
                    cnt++;
            }
            if (cnt != rParams.size() - 1)
                errorHandling->printfError(func);
        }
    }
}

void CallExp::traverse(int lev) {
    checkPrintf();
    table.checkFunc(this);
    cout << func.val << "(";
    for (int i = 0; i < rParams.size(); i++) {
        if (i > 0) cout << ", ";
        rParams[i]->traverse(lev);
    }
    cout << ")";
}

Type CallExp::evalType() {
    Func* func = table.findFunc(this);
    return func->getType();
}

int CallExp::evalInt() {
    cout << "only const can eval" << endl;
    return 0;
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

Type UnaryExp::evalType() {
    return Exp::evalType();
}

int UnaryExp::evalInt() {
    if (op.empty()) {
        if (sym.sym == INTCON) return stoi(sym.val);
        else return exp->evalInt();
    }
    return 0;
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

Symbol LVal::getName() {
    return name;
}

void LVal::traverse(int lev) {
    table.checkDecl(this);
    cout << name.val;
    for (auto x : dims) {
        cout << "[";
        x->traverse(lev);
        cout << "]";
    }
}

Type LVal::evalType() {
    Decl* decl = table.findDecl(this);
    return decl->getType();
}

int LVal::evalInt() {
    Decl* decl = table.findDecl(this);
    if (decl->isConst()) {
        if (dims.empty()) return decl->getInitVal()[0];
        else {
            int i = 0, last = 1;
            for (auto x : dims) i = last*i + x->evalInt();
        }
    } else {
        cout << "only const can eval" << endl;
        return 0;
    }
    return 0;
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

Type Exp::evalType() {
    return Type();
}

int Exp::evalInt() {
    return 0;
}

AssignExp::AssignExp() {
    lhs = nullptr;
    rhs = nullptr;
}

AssignExp::AssignExp(LVal *l, Exp *r) {
    lhs = l;
    rhs = r;
}

void AssignExp::checkConst() {
    Decl* decl = table.findDecl(lhs);
    if (decl == nullptr) return;
    else if (decl->isConst()) errorHandling->constAssign(lhs->getName());
}

void AssignExp::traverse(int lev) {
    checkConst();
    lhs->traverse(lev);
    cout << " = ";
    rhs->traverse(lev);
}

Type AssignExp::evalType() {
    if (!(lhs->evalType() == rhs->evalType())); // check lhs and rhs type
    return lhs->evalType();
}

int AssignExp::evalInt() {
    cout << "only const can eval" << endl;
    return rhs->evalInt();
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

Symbol CondStmt::getSym() {
    return sym;
}

Stmt *CondStmt::getIfStmt() {
    return ifStmt;
}

Stmt *CondStmt::getElseStmt() {
    return elseStmt;
}

void CondStmt::traverse(int lev) {
    for (int i = 0; i < lev; i++) cout << "    ";
    cout << sym.val << " (";
    condExp->traverse(lev);
    cout << ")" << endl;
    table.pushBlock();
    if (auto* blk = dynamic_cast<Block*>(ifStmt)) {
        ifStmt->traverse(lev);
    } else {
        ifStmt->traverse(lev + 1);
    }
    table.popBlock();
    table.pushBlock();
    if (elseStmt) elseStmt->traverse(lev);
    table.popBlock();
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

Symbol ReturnStmt::getSymbol() {
    return sym;
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
    table.addError(errorHandling);
}

Ast::Ast(Program* p, ErrorHandling* err) {
    program = p;
    errorHandling = err;
    table.addError(err);
}

void Ast::traverse() {
    program->addError(errorHandling);
    program->traverse(0);
}
