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

}

Decl::Decl(Type bType, Symbol ident) {
    this->bType = move(bType);
    this->ident = move(ident);
}

Decl::Decl(Type bType, Symbol ident, std::vector<Exp *> initVal={}) {
    this->bType = move(bType);
    this->ident = move(ident);
    this->initVal.insert(this->initVal.end(), initVal.begin(), initVal.end());
}

bool Decl::isConst() {
    return bType.getConst();
}

bool Decl::isParam() {
    return bType.getParam();
}

bool Decl::isPointer() {
    return bType.getPointer();
}

Type Decl::getType() {
    return bType;
}

int Decl::getDim() {
    return bType.getDim();
}

vector<int> Decl::getDims() {
    return bType.getDims();
}

Symbol Decl::getIdent() {
    return ident;
}

std::vector<int> Decl::getInitVal() {
    vector<int> v;
    for (auto x : initVal) {
        v.push_back(x->evalInt());
    }
    return v;
}

void Decl::traverse(int lev) {
    table.pushDecl(this);
    if (!bType.getParam()) {
        for (int i = 0; i < lev; i++)
            cout << "    ";
    }
    if (bType.getConst()) cout << "const ";
    cout << bType.getType().val << " ";
    cout << ident.val;
    if (bType.getPointer()) cout << "[]";
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
    if (!bType.getParam()) cout << ";" << endl;
}

Func::Func() {
    block = nullptr;
}

Func::Func(Type returnType, Symbol ident, std::vector<Decl *> fParams, Block *block) {
    this->returnType = move(returnType);
    this->ident = move(ident);
    this->fParams.insert(this->fParams.end(), fParams.begin(), fParams.end());
    this->block = block;
}

Symbol Func::getIdent() {
    return ident;
}

std::vector<Decl *> Func::getParams() {
    return fParams;
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
    cout << ident.val << " (";
    for (int i = 0; i < fParams.size(); i++) {
        if (i > 0) cout << ", ";
        fParams[i]->traverse(lev);
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

BinaryExp::BinaryExp(Exp *lhs, Symbol op, Exp *rhs) {
    this->lhs = lhs;
    this->op  = move(op);
    this->rhs = rhs;
}

Symbol BinaryExp::getVal() {
    return op;
}

void BinaryExp::traverse(int lev) {
    lhs->traverse(lev);
    cout << " " << op.val << " ";
    rhs->traverse(lev);
}

Type BinaryExp::evalType() {
    if (lhs->evalType() == rhs->evalType())
        return lhs->evalType();
    return Exp::evalType();
}

int BinaryExp::evalInt() {
    if (op.sym == PLUS) {
        return lhs->evalInt() + rhs->evalInt();
    } else if (op.sym == MINU) {
        return lhs->evalInt() - rhs->evalInt();
    } else if (op.sym == MULT) {
        return lhs->evalInt() * rhs->evalInt();
    } else if (op.sym == DIV) {
        return lhs->evalInt() / rhs->evalInt();
    } else if (op.sym == MOD) {
        return lhs->evalInt() % rhs->evalInt();
    } else if (op.sym == AND) {
        return lhs->evalInt() && rhs->evalInt();
    } else if (op.sym == OR) {
        return lhs->evalInt() || rhs->evalInt();
    } else if (op.sym == LSS) {
        return lhs->evalInt() < rhs->evalInt();
    } else if (op.sym == LEQ) {
        return lhs->evalInt() <= rhs->evalInt();
    } else if (op.sym == GRE) {
        return lhs->evalInt() > rhs->evalInt();
    } else if (op.sym == GEQ) {
        return lhs->evalInt() >= rhs->evalInt();
    } else if (op.sym == EQL) {
        return lhs->evalInt() == rhs->evalInt();
    } else if (op.sym == NEQ) {
        return lhs->evalInt() != rhs->evalInt();
    }
    return 0;
}

Program::Program() {

}

Program::Program(std::vector<Decl *> decls, std::vector<Func *> funcs) {
    this->program_items.insert(this->program_items.end(), decls.begin(), decls.end());
    this->program_items.insert(this->program_items.end(), funcs.begin(), funcs.end());
}

void Program::addError(ErrorHandling* error) {
    err = error;
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
    loop = false;
}

Block::Block(Symbol lBrace, vector<BlockItem*> items, Symbol rBrace) {
    this->lBrace = move(lBrace);
    block_items.insert(block_items.end(), items.begin(), items.end());
    this->rBrace = move(rBrace);
}

Symbol Block::getLBrace() {
    return lBrace;
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
            if (w->getToken().sym == IFTK) {
                if (auto* blk = dynamic_cast<Block*>(w->getIfStmt())) {
                    blk->checkLoop();
                } else if (auto* loop = dynamic_cast<LoopStmt*>(w->getIfStmt())) {
                    errorHandling->loopError(loop->getSym());
                }
                if (w->getElseStmt()) {
                    if (auto* blk = dynamic_cast<Block*>(w->getElseStmt())) {
                        blk->checkLoop();
                    } else if (auto* loop = dynamic_cast<LoopStmt*>(w->getElseStmt())) {
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

CallExp::CallExp(Symbol ident) {
    this->ident = ident;
}

CallExp::CallExp(Symbol ident, vector<Exp*> rParams) {
    this->ident = ident;
    this->rParams.insert(this->rParams.end(), rParams.begin(), rParams.end());
}

std::vector<Exp *> CallExp::getParams() {
    return rParams;
}

bool CallExp::isGetInt() const {
    return ident.sym == GETINTTK;
}

bool CallExp::isPrintf() const {
    return ident.sym == PRINTFTK;
}

Symbol CallExp::getIdent() {
    return ident;
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
                errorHandling->printfError(ident);
        }
    }
}

void CallExp::traverse(int lev) {
    checkPrintf();
    table.checkFunc(this);
    cout << ident.val << "(";
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

ExpStmt::ExpStmt(Exp *e) {
    this->exp = e;
}

void ExpStmt::traverse(int lev) {
    for (int i = 0; i < lev; i++) cout << "    ";
    exp->traverse(lev);
    cout << ";" << endl;
}

UnaryExp::UnaryExp() {
    this->op = nullptr;
    this->exp = nullptr;
}

UnaryExp::UnaryExp(Exp *exp) {
    this->op = nullptr;
    this->exp = exp;
}

UnaryExp::UnaryExp(Symbol sym) {
    this->op = nullptr;
    this->exp = nullptr;
    this->sym = move(sym);
}

UnaryExp::UnaryExp(Symbol op, Symbol sym) {
    this->op = new Symbol;
    *(this->op) = move(op);
    this->sym = move(sym);
    this->exp = nullptr;
}

UnaryExp::UnaryExp(Symbol op, Exp *exp) {
    this->op = new Symbol;
    *(this->op) = move(op);
    this->exp = exp;
}

Exp *UnaryExp::getExp() {
    return exp;
}

Symbol UnaryExp::getOp() {
    return *op;
}

void UnaryExp::traverse(int lev) {
    if (op) cout << op->val;
    if (exp) exp->traverse(lev);
    else cout << sym.val;
}

Type UnaryExp::evalType() {
    if (sym.sym == INTCON)
        return Type(Symbol(INTTK, "int", sym.row, sym.col));
    return exp->evalType();
}

int UnaryExp::evalInt() {
    if (op == nullptr) {
        if (sym.sym == INTCON) return stoi(sym.val);
        else return exp->evalInt();
    }
    return 0;
}

Stmt::Stmt() {

}

void Stmt::traverse(int lev) {

}

LVal::LVal() {

}

LVal::LVal(Symbol sym, std::vector<Exp *> dims = {}) {
    this->ident = move(sym);
    this->dims.insert(this->dims.end(), dims.begin(), dims.end());
}

Symbol LVal::getIdent() {
    return ident;
}

void LVal::traverse(int lev) {
    table.checkDecl(this);
    cout << ident.val;
    for (auto x : dims) {
        cout << "[";
        x->traverse(lev);
        cout << "]";
    }
}

Type LVal::evalType() {
    Decl* decl = table.findDecl(this);
    if (decl == nullptr) return {};

    Type declType = decl->getType();
    Symbol tkn = declType.getType();
    vector<int> declDim = decl->getDims();

    Type type = Type(tkn);

    int n = declDim.size();
    int lValDim = n - dims.size();
    for (int i = 0; i < lValDim; i++) {
        type.addDim(declDim[n - i - 1]);
    }

    return type;
}

int LVal::evalInt() {
    Decl* decl = table.findDecl(this);
    if (decl->getType().getConst()) {
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
    else if (decl->isConst()) errorHandling->constAssign(lhs->getIdent());
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

CondStmt::CondStmt(Symbol token, Exp *condExp, Stmt *ifStmt, Stmt *elseStmt) {
    this->token = move(token);
    this->condExp = condExp;
    this->ifStmt = ifStmt;
    this->elseStmt = elseStmt;
}

Symbol CondStmt::getToken() {
    return token;
}

Stmt *CondStmt::getIfStmt() {
    return ifStmt;
}

Stmt *CondStmt::getElseStmt() {
    return elseStmt;
}

void CondStmt::traverse(int lev) {
    for (int i = 0; i < lev; i++) cout << "    ";
    cout << token.val << " (";
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
    if (elseStmt) {
        for (int i = 0; i < lev; i++) cout << "    ";
        cout << "else ";
        elseStmt->traverse(lev);
    }
    table.popBlock();
}

LoopStmt::LoopStmt() {

}

LoopStmt::LoopStmt(Symbol sym) {
    this->sym = move(sym);
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

ReturnStmt::ReturnStmt(Symbol sym, Exp* exp = nullptr) {
    this->sym = move(sym);
    this->exp = exp;
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
