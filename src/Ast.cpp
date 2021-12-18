//
// Created by tjan2 on 17/10/2021.
//

#include <iostream>
#include <utility>
#include "Ast.h"
#include "FrontEnd/Table.h"

using namespace std;

Table table;
ErrorHandling* errorHandling;
Context *Ast::ctx;
BlockItem::BlockItem() {

}

void BlockItem::traverse(int lev) {

}

void BlockItem::generateCode() {

}

ProgramItem::ProgramItem() {

}

void ProgramItem::traverse(int lev) {

}

void ProgramItem::generateCode() {

}

Decl::Decl() {

}

Decl::Decl(Type bType, Symbol ident) {
    this->bType = move(bType);
    this->ident = move(ident);
}

Decl::Decl(Type bType, Symbol ident, std::vector<Exp *> initVal) {
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

std::vector<Exp *> Decl::getInitValExp() {
    return initVal;
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

bool Decl::isArray() {
    return bType.getDim() > 0;
}

bool Decl::hasInit() {
    return !initVal.empty();
}

void Decl::generateCode() {
    table.pushDecl(this);

    if (isArray()) {
        if (isParam()) {
            int base = Ast::ctx->func->genVar();
            auto ptr = new IrPointer(base, this);
            if (isConst()) ptr->addConst();
            auto decl = new DeclInst(ptr);
            Ast::ctx->func->addDecl(this, decl);
            Ast::ctx->blk->addInst(decl);
        } else {
            bool isGlobal = Ast::ctx->table->isGlobal();
            int size = 1;
            for (auto x : bType.getDims()) size *= x;
            int base = isGlobal ? Ast::ctx->genArray(size) : Ast::ctx->func->genArray(size);
            auto arr = new IrArray(base, isGlobal, bType);

            if (isConst()) arr->addConst();
            auto decl = new DeclInst(arr);
            if (hasInit()) {
                vector<Variable*> inits;
                for (auto x : initVal) {
                    if (isConst() || isGlobal) {
                        inits.push_back(new Constant(x->evalInt()));
                    } else {
                        x->generateCode();
                        if (x->getVar()->isAddr()) {
                            auto val = new Variable(Ast::ctx->func->genVar(), false, x->evalType());
                            Ast::ctx->blk->addInst(new LoadInst(val, x->getVar()));
                            inits.push_back(val);
                        } else {
                            inits.push_back(x->getVar());
                        }
                    }
                }
                decl->addInits(inits);
            }
            if (isGlobal) {
                Ast::ctx->module->addDecl(this, decl);
            } else {
                Ast::ctx->func->addDecl(this, decl);
                Ast::ctx->blk->addInst(decl);
            }
        }
    } else {
        if (isConst()) {
            Ast::ctx->addConst(this);
            return;
        }

        Variable* var;
        bool isGlobal = Ast::ctx->table->isGlobal();
        int id = isGlobal ? Ast::ctx->genGlobal() : Ast::ctx->func->genVar();
        if (Ast::ctx->isParam) var = new IrParam(id, this);
        else var = new Variable(id, isGlobal, bType);
        auto decl = new DeclInst(var);

        if (hasInit()) {
            if (isConst() || isGlobal) {
                decl->addInit(new Constant(initVal[0]->evalInt()));
            } else {
                initVal[0]->generateCode();
                if (initVal[0]->getVar()->isAddr()) {
                    auto val = new Variable(Ast::ctx->func->genVar(), false, initVal[0]->evalType());
                    Ast::ctx->blk->addInst(new LoadInst(val, initVal[0]->getVar()));
                    decl->addInit(val);
                } else {
                    decl->addInit(initVal[0]->getVar());
                }
            }
        }

        if (isGlobal) {
            Ast::ctx->module->addDecl(this, decl);
        } else {
            Ast::ctx->func->addDecl(this, decl);
            Ast::ctx->blk->addInst(decl);
        }
    }
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

void Func::generateCode() {
    table.pushFunc(this);
    table.pushBlock();

    auto irFunc    = new IrFunc(this);
    int  label_id  = Ast::ctx->genLabel();
    auto entry_blk = new BasicBlock(label_id);
    Ast::ctx->module->addFunc(irFunc);
    irFunc->addBlock(entry_blk);
    Ast::ctx->addLabel(label_id, entry_blk);

    Ast::ctx->func = irFunc;
    // generate function parameters
    Ast::ctx->isParam = true;
    Ast::ctx->blk = irFunc->getEntryBlock();
    for (auto x : fParams)
        x->generateCode();
    Ast::ctx->isParam = false;

    block->generateCode();
    auto instr = Ast::ctx->blk->getLastInst();
    auto retInstr = dynamic_cast<ReturnInst*>(instr);
    // if block don't have return instruction, add return instruction on IR
    if (!retInstr) Ast::ctx->blk->addInst(new ReturnInst());

    Ast::ctx->blk = nullptr;
    Ast::ctx->func = nullptr;
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

Symbol BinaryExp::getOp() {
    return op;
}

Exp *BinaryExp::getLhs() {
    return lhs;
}

Exp *BinaryExp::getRhs() {
    return rhs;
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

void BinaryExp::generateCode() {
    auto sym = this->op.sym;
    if (sym == AND) {
        int end_label = Ast::ctx->genLabel();
        auto end_blk = new BasicBlock(end_label);
        int right_label = Ast::ctx->genLabel();
        auto right_blk = new BasicBlock(right_label);
        auto var = new Variable(Ast::ctx->func->genVar(), false, evalType());
        lhs->generateCode();
        if (lhs->getVar()->isAddr()) {
            auto val = new Variable(Ast::ctx->func->genVar(), false, evalType());
            Ast::ctx->blk->addInst(new LoadInst(val, lhs->getVar()));
            Ast::ctx->blk->addInst(new BranchInst(Bnez, val, right_label));
        } else {
            Ast::ctx->blk->addInst(new BranchInst(Bnez, lhs->getVar(), right_label));
        }
        auto blk = new BasicBlock(Ast::ctx->genLabel());
        Ast::ctx->func->addBlock(blk);
        Ast::ctx->blk = blk;
        Ast::ctx->blk->addInst(new AssignInst(var, new Constant(0)));
        Ast::ctx->blk->addInst(new JumpInst(end_label));
        Ast::ctx->func->addBlock(right_blk);
        Ast::ctx->blk = right_blk;
        rhs->generateCode();
        if (rhs->getVar()->isAddr()) {
            auto val = new Variable(Ast::ctx->func->genVar(), false, evalType());
            Ast::ctx->blk->addInst(new LoadInst(val, rhs->getVar()));
            Ast::ctx->blk->addInst(new AssignInst(var, val));
        } else {
            Ast::ctx->blk->addInst(new AssignInst(var, rhs->getVar()));
        }
        Ast::ctx->func->addBlock(end_blk);
        Ast::ctx->blk = end_blk;
        this->addVar(var);
    } else if (sym == OR) {
        int end_label = Ast::ctx->genLabel();
        auto end_blk = new BasicBlock(end_label);
        int right_label = Ast::ctx->genLabel();
        auto right_blk = new BasicBlock(right_label);
        auto var = new Variable(Ast::ctx->func->genVar(), false, evalType());
        lhs->generateCode();
        if (lhs->getVar()->isAddr()) {
            auto val = new Variable(Ast::ctx->func->genVar(), false, evalType());
            Ast::ctx->blk->addInst(new LoadInst(val, lhs->getVar()));
            Ast::ctx->blk->addInst(new BranchInst(Beqz, val, right_label));
        } else {
            Ast::ctx->blk->addInst(new BranchInst(Beqz, lhs->getVar(), right_label));
        }
        auto blk = new BasicBlock(Ast::ctx->genLabel());
        Ast::ctx->func->addBlock(blk);
        Ast::ctx->blk = blk;
        Ast::ctx->blk->addInst(new AssignInst(var, new Constant(1)));
        Ast::ctx->blk->addInst(new JumpInst(end_label));
        Ast::ctx->func->addBlock(right_blk);
        Ast::ctx->blk = right_blk;
        rhs->generateCode();
        if (rhs->getVar()->isAddr()) {
            auto val = new Variable(Ast::ctx->func->genVar(), false, evalType());
            Ast::ctx->blk->addInst(new LoadInst(val, rhs->getVar()));
            Ast::ctx->blk->addInst(new AssignInst(var, val));
        } else {
            Ast::ctx->blk->addInst(new AssignInst(var, rhs->getVar()));
        }
        Ast::ctx->func->addBlock(end_blk);
        Ast::ctx->blk = end_blk;
        this->addVar(var);
    } else {
        lhs->generateCode();
        rhs->generateCode();
        auto var = new Variable(Ast::ctx->expStmt->genVar(), false, evalType());
        if (lhs->getVar()->isAddr() && rhs->getVar()->isAddr()) {
            auto lval = new Variable(Ast::ctx->expStmt->genVar(), false, lhs->evalType());
            auto rval = new Variable(Ast::ctx->expStmt->genVar(), false, rhs->evalType());
            Ast::ctx->blk->addInst(new LoadInst(lval, lhs->getVar()));
            Ast::ctx->blk->addInst(new LoadInst(rval, rhs->getVar()));
            Ast::ctx->blk->addInst(new BinaryInst(var, lval, op, rval));
            this->addVar(var);
        } else if (lhs->getVar()->isAddr()) {
            auto lval = new Variable(Ast::ctx->expStmt->genVar(), false, lhs->evalType());
            Ast::ctx->blk->addInst(new LoadInst(lval, lhs->getVar()));
            Ast::ctx->blk->addInst(new BinaryInst(var, lval, op, rhs->getVar()));
            this->addVar(var);
        } else if (rhs->getVar()->isAddr()) {
            auto rval = new Variable(Ast::ctx->expStmt->genVar(), false, rhs->evalType());
            Ast::ctx->blk->addInst(new LoadInst(rval, rhs->getVar()));
            Ast::ctx->blk->addInst(new BinaryInst(var, lhs->getVar(), op, rval));
            this->addVar(var);
        } else {
            auto inst = new BinaryInst(var, this);
            Ast::ctx->blk->addInst(inst);
            this->addVar(var);
        }
    }
}

Program::Program() {
    this->err = nullptr;
}

Program::Program(std::vector<ProgramItem *> items) {
    this->err = nullptr;
    this->program_items.insert(this->program_items.end(), items.begin(), items.end());
}

void Program::addError(ErrorHandling* error) {
    err = error;
}

void Program::traverse(int lev) {
    table.pushBlock();
    for (auto node : program_items) {
        node->traverse(lev);
    }
    table.popBlock();
}

void Program::generateCode() {
    table.pushBlock();
    for (auto node : program_items) {
        node->generateCode();
    }
    table.popBlock();
}

Block::Block() {

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

void Block::generateCode() {
    for (auto x : block_items) {
        if (auto blk = dynamic_cast<Block*>(x)) {
            table.pushBlock();
            blk->generateCode();
            table.popBlock();
        } else {
            x->generateCode();
        }
    }
}

CallExp::CallExp() {

}

CallExp::CallExp(Symbol ident) {
    this->ident = move(ident);
}

CallExp::CallExp(Symbol ident, vector<Exp*> rParams) {
    this->ident = move(ident);
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
    if (isGetInt()) return Type(Symbol(INTTK, "int", ident.row, ident.col));
    Func* func = table.findFunc(this);
    return func->getType();
}

int CallExp::evalInt() {
    cout << "only const can eval" << endl;
    return 0;
}

std::vector<Variable*> split(std::vector<Exp*> rParams) {
    auto fstring = dynamic_cast<FormatString*>(rParams[0]);
    auto str = fstring->getSym().val;
    // remove ""
    str = str.substr(1, str.size() - 2);
    int start = 0, end = (int) str.find("%d");
    vector<Variable*> v;
    int i = 1;
    if (end == -1) {  // if fstring don't have %d
        Ast::ctx->module->addStr(str);
        v.push_back(new IrParam(new Constant(str)));
    } else {
        while (end != -1) {
            auto s = str.substr(start, end - start);
            if (!s.empty()) {
                Ast::ctx->module->addStr(s);
                v.push_back(new IrParam(new Constant(s)));
            }
            rParams[i]->generateCode();
            auto var = rParams[i++]->getVar();
            if (var->isAddr()) {
                auto val = new Variable(Ast::ctx->func->genVar(), false, var->getType());
                Ast::ctx->blk->addInst(new LoadInst(val, var));
                v.push_back(new IrParam(val));
            } else if (auto constant = dynamic_cast<Constant *>(var)) {
                v.push_back(new IrParam(constant));
            } else {
                v.push_back(new IrParam(var));
            }

            start = end + 2;
            end = (int) str.find("%d", start);
            if (end == -1 && start < str.size()) {
                Ast::ctx->module->addStr(str.substr(start));
                v.push_back(new IrParam(new Constant(str.substr(start))));
            }
        }
    }

    return v;
}

void CallExp::generateCode() {
    if (isPrintf()) { //  printf()
        auto p = split(rParams);
        auto inst = new CallInst(ident, p);
        Ast::ctx->blk->addInst(inst);
    } else if (isGetInt()) { //  getint()
        Ast::ctx->blk->addInst(new CallInst(ident));
        int id = Ast::ctx->func->genVar();
        auto var = new Variable(id, false, this->evalType());
        Ast::ctx->blk->addInst(new GetReturnInst(var));
        this->addVar(var);
    } else { //  other function
        vector<Variable*> params;
        auto func = Ast::ctx->getFunc(ident.val);
        if (rParams.empty()) { //  if function don't have parameters
            Ast::ctx->blk->addInst(new CallInst(func));
        } else {
            for (auto x : rParams) {
                x->generateCode();
                auto var = x->getVar();
                if (var->isAddr()) {
                    auto val = new Variable(Ast::ctx->func->genVar(), false, var->getType());
                    Ast::ctx->blk->addInst(new LoadInst(val, var));
                    params.push_back(val);
                } else {
                    params.push_back(var);
                }
            }
            Ast::ctx->blk->addInst(new CallInst(func, params));
        }
        //  if this function has return value
        if (func->getType().getType().sym != VOIDTK) {
            int id = Ast::ctx->func->genVar();
            auto var = new Variable(id, false, this->evalType());
            Ast::ctx->blk->addInst(new GetReturnInst(var));
            this->addVar(var);
        }
    }
}

ExpStmt::ExpStmt() {
    exp = nullptr;
    this->varId = 0;
}

ExpStmt::ExpStmt(Exp *e) {
    this->exp = e;
    this->varId = 0;
}

int ExpStmt::genVar() {
    if (Ast::ctx->table->isGlobal()) return varId++;
    else return Ast::ctx->func->genVar();
}

void ExpStmt::traverse(int lev) {
    for (int i = 0; i < lev; i++) cout << "    ";
    exp->traverse(lev);
    cout << ";" << endl;
}

void ExpStmt::generateCode() {
    Ast::ctx->expStmt = this;
    exp->generateCode();
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

Symbol* UnaryExp::getOp() {
    return op;
}

Symbol UnaryExp::getSymbol() {
    return sym;
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
    if (op->sym == PLUS) {
        return exp->evalInt();
    } else if (op->sym == MINU) {
        return -1 * exp->evalInt();
    } else if (op->sym == NOT) {
        return !exp->evalInt();
    }
    return 0;
}

void UnaryExp::generateCode() {
    if (op == nullptr) {
        if (exp) {
            exp->generateCode();
            if (exp->getVar()->isAddr()) {
                auto var = new Variable(Ast::ctx->expStmt->genVar(), false, evalType());
                Ast::ctx->blk->addInst(new LoadInst(var, exp->getVar()));
                this->addVar(var);
            } else {
                this->addVar(exp->getVar());
            }
        } else if (sym.sym == INTCON) { //  this Exp is an integer
            this->addVar(new Constant(stoi(sym.val)));
        } else {
            // taktau
        }
    } else {
        if (op->sym == PLUS) {
            exp->generateCode();
            if (exp->getVar()->isAddr()) {
                auto val = new Variable(Ast::ctx->expStmt->genVar(), false, evalType());
                Ast::ctx->blk->addInst(new LoadInst(val, exp->getVar()));
                this->addVar(val);
            } else {
                this->addVar(exp->getVar());
            }
        } else if (op->sym == MINU) {
            exp->generateCode();
            if (exp->getVar()->isAddr()) {
                auto val = new Variable(Ast::ctx->func->genVar(), false, exp->evalType());
                Ast::ctx->blk->addInst(new LoadInst(val, exp->getVar()));
                auto var = new Variable(Ast::ctx->expStmt->genVar(), false, exp->evalType());
                Ast::ctx->blk->addInst(new BinaryInst(var, Sub, new Constant(0), val));
                this->addVar(var);
            } else {
                auto var = new Variable(Ast::ctx->expStmt->genVar(), false, exp->evalType());
                Ast::ctx->blk->addInst(new BinaryInst(var, Sub, exp));
                this->addVar(var);
            }
        } else if (op->sym == NOT) {
            exp->generateCode();
            if (exp->getVar()->isAddr()) {
                auto val = new Variable(Ast::ctx->func->genVar(), false, exp->evalType());
                Ast::ctx->blk->addInst(new LoadInst(val, exp->getVar()));
                auto var = new Variable(Ast::ctx->func->genVar(), false, exp->evalType());
                Ast::ctx->blk->addInst(new NotInst(val, var));
                this->addVar(var);
            } else {
                auto var = new Variable(Ast::ctx->func->genVar(), false, exp->evalType());
                Ast::ctx->blk->addInst(new NotInst(exp->getVar(), var));
                this->addVar(var);
            }
        }
    }
}

Stmt::Stmt() {

}

void Stmt::traverse(int lev) {

}

void Stmt::generateCode() {

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

std::vector<Exp *> LVal::getDims() {
    return dims;
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

    int n = (int) declType.getDim();
    int lValDim = n - (int) dims.size();
    for (int i = 0; i < lValDim; i++) {
        if (declType.getPointer()) {
            if (i == n - 1 && declType.getPointer()) type.addPointer();
            else type.addDim(declDim[n - i - 2]);
        } else {
            type.addDim(declDim[n - i - 1]);
        }
    }
    if (declType.getParam()) type.addParam();

    return type;
}

int LVal::evalInt() {
    Decl* decl = table.findDecl(this);
    if (decl->getType().getConst()) {
        if (dims.empty()) return decl->getInitVal()[0];
        else {
            auto d = decl->getDims();
            int offset = 0;
            for (int i = 0; i < dims.size(); i++) {
                offset = offset * d[i] + dims[i]->evalInt();
            }
            return decl->getInitVal()[offset];
        }
    } else if (Ast::ctx->table->isGlobal()) {
        if (dims.empty()) return decl->getInitVal()[0];
        else {
            auto d = decl->getDims();
            int offset = 0;
            for (int i = 0; i < dims.size(); i++) {
                offset = offset * d[i] + dims[i]->evalInt();
            }
            return decl->getInitVal()[offset];
        }
    }
    return 0;
}

void LVal::generateCode() {
    Decl* decl = table.findDecl(this);
    if (decl->isArray()) {
        auto base = Ast::ctx->func->getVar(decl);
        // is global variable
        if (base == nullptr) base = Ast::ctx->module->getGlobal(decl);
        auto var = new Variable(Ast::ctx->func->genVar(), false, base->getType());
        if (auto ptr = dynamic_cast<IrPointer*>(base)) Ast::ctx->blk->addInst(new AssignInst(var, base));
        else Ast::ctx->blk->addInst(new LoadAddrInst(var, base));
        if (dims.empty()) {
            this->addVar(var);
        } else {
            Variable *prev = nullptr;
            if (dims.size() < decl->getDim()) {
                int n = decl->getDim();
                auto offset = new Variable(Ast::ctx->func->genVar(), false, dims[0]->evalType());
                for (int i = 0; i < n - 1; i++) {
                    Constant* size;
                    if (auto ptr = dynamic_cast<IrPointer*>(base)) {
                        size = new Constant(decl->getDims()[i]);
                    } else {
                        size = new Constant(decl->getDims()[i + 1]);
                    }
                    if (i < dims.size()) {
                        dims[i]->generateCode();
                        if (i == 0) {
                            if (dims[i]->getVar()->isAddr()) {
                                auto val = new Variable(Ast::ctx->func->genVar(), false, dims[i]->evalType());
                                Ast::ctx->blk->addInst(new LoadInst(val, dims[i]->getVar()));
                                Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, val, size));
                            } else {
                                Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, dims[i]->getVar(), size));
                            }
                        } else {
                            if (dims[i]->getVar()->isAddr()) {
                                auto val = new Variable(Ast::ctx->func->genVar(), false, dims[i]->evalType());
                                Ast::ctx->blk->addInst(new LoadInst(val, dims[i]->getVar()));
                                Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, val, size));
                            } else {
                                Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, dims[i]->getVar(), size));
                            }
                            Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, offset, size));
                        }
                    } else {
                        Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, offset, size));
                    }
                }
                Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, offset, new Constant(4)));
                Ast::ctx->blk->addInst(new BinaryInst(offset, Add, var, offset));
                this->addVar(offset);
            } else {
                for (int i = 0; i < dims.size(); i++) {
                    int id = Ast::ctx->func->genVar();
                    auto offset = new Variable(id, false, dims[i]->evalType());
                    if (i == dims.size() - 1) {
                        dims[i]->generateCode();
                        if (prev == nullptr) {
                            if (dims[i]->getVar()->isAddr()) {
                                auto val = new Variable(Ast::ctx->func->genVar(), false, dims[i]->evalType());
                                Ast::ctx->blk->addInst(new LoadInst(val, dims[i]->getVar()));
                                prev = val;
                            } else {
                                prev = dims[i]->getVar();
                            }
                        }
                        Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, prev, new Constant(4)));
                        Ast::ctx->blk->addInst(new BinaryInst(offset, Add, var, offset));
                    } else {
                        if (auto ptr = dynamic_cast<IrPointer*>(base)) {
                            auto size = new Constant(decl->getDims()[i]);
                            if (prev == nullptr) {
                                dims[i]->generateCode();
                                if (dims[i]->getVar()->isAddr()) {
                                    auto val = new Variable(Ast::ctx->func->genVar(), false, dims[i]->evalType());
                                    Ast::ctx->blk->addInst(new LoadInst(val, dims[i]->getVar()));
                                    Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, val, size));
                                } else {
                                    Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, dims[i]->getVar(), size));
                                }
                            } else {
                                Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, prev, size));
                            }
                            dims[i + 1]->generateCode();
                            if (dims[i + 1]->getVar()->isAddr()) {
                                auto val = new Variable(Ast::ctx->func->genVar(), false, dims[i + 1]->evalType());
                                Ast::ctx->blk->addInst(new LoadInst(val, dims[i + 1]->getVar()));
                                Ast::ctx->blk->addInst(new BinaryInst(offset, Add, offset, val));
                            } else {
                                Ast::ctx->blk->addInst(new BinaryInst(offset, Add, offset, dims[i + 1]->getVar()));
                            }
                        } else {
                            auto size = new Constant(decl->getDims()[i + 1]);
                            dims[i + 1]->generateCode();
                            if (prev == nullptr) {
                                dims[i]->generateCode();
                                if (dims[i]->getVar()->isAddr()) {
                                    auto val = new Variable(Ast::ctx->func->genVar(), false, evalType());
                                    Ast::ctx->blk->addInst(new LoadInst(val, dims[i]->getVar()));
                                    Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, val, size));
                                } else {
                                    Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, dims[i]->getVar(), size));
                                }
                            } else {
                                Ast::ctx->blk->addInst(new BinaryInst(offset, Mul, prev, size));
                            }
                            if (dims[i + 1]->getVar()->isAddr()) {
                                auto val = new Variable(Ast::ctx->func->genVar(), false, evalType());
                                Ast::ctx->blk->addInst(new LoadInst(val, dims[i + 1]->getVar()));
                                Ast::ctx->blk->addInst(new BinaryInst(offset, Add, offset, val));
                            } else {
                                Ast::ctx->blk->addInst(new BinaryInst(offset, Add, offset, dims[i + 1]->getVar()));
                            }
                        }
                    }
                    prev = offset;
                }
                prev->addAddr();
                this->addVar(prev);
            }
        }
    } else if (decl->isConst()) {
        this->addVar(Ast::ctx->const_map[decl]);
    } else if (Ast::ctx->func && Ast::ctx->func->getVar(decl)) {
        auto var = Ast::ctx->func->getVar(decl);
        this->addVar(var);
    } else {
        auto var = Ast::ctx->module->getGlobal(decl);
        this->addVar(var);
    }
}

Exp::Exp() {
    cond = false;
    var  = nullptr;
}

void Exp::addCond() {
    cond = true;
}

bool Exp::isCond() const {
    return cond;
}

int Exp::evalInt() {
    return 0;
}

Type Exp::evalType() {
    return {};
}

void Exp::traverse(int lev) {

}

void Exp::generateCode() {

}

void Exp::addVar(Variable *var) {
    this->var = var;
}

Variable *Exp::getVar() {
    return var;
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

void AssignExp::generateCode() {
    lhs->generateCode();
    rhs->generateCode();
    if (lhs->getVar()->isAddr() && rhs->getVar()->isAddr()) {
        auto rval = new Variable(Ast::ctx->func->genVar(), false, rhs->evalType());
        Ast::ctx->blk->addInst(new LoadInst(rval, rhs->getVar()));
        auto lval = new Variable(Ast::ctx->func->genVar(), false, rhs->evalType());
        Ast::ctx->blk->addInst(new AssignInst(lval, rval));
        Ast::ctx->blk->addInst(new StoreInst(lval, lhs->getVar()));
    } else if (lhs->getVar()->isAddr()) {
        auto val = new Variable(Ast::ctx->func->genVar(), false, lhs->evalType());
        Ast::ctx->blk->addInst(new LoadInst(val, lhs->getVar()));
        Ast::ctx->blk->addInst(new AssignInst(val, rhs->getVar()));
        Ast::ctx->blk->addInst(new StoreInst(val, lhs->getVar()));
    } else if (rhs->getVar()->isAddr()) {
        auto val = new Variable(Ast::ctx->func->genVar(), false, rhs->evalType());
        Ast::ctx->blk->addInst(new LoadInst(val, rhs->getVar()));
        Ast::ctx->blk->addInst(new AssignInst(lhs->getVar(), val));
    } else {
        this->addVar(lhs->getVar());
        Ast::ctx->blk->addInst(new AssignInst(lhs->getVar(), rhs->getVar()));
    }
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

#define IF 1
#define IF_BODY 2
#define ELSE 3
#define IF_END 5

BasicBlock* setupIfBlock(int id, int type) {
    auto blk = new BasicBlock(id);
    switch(type) {
        case IF:        Ast::ctx->if_blk = blk;        break;
        case IF_BODY:   Ast::ctx->if_body_blk = blk;      break;
        case ELSE:      Ast::ctx->else_blk = blk;      break;
        case IF_END:    Ast::ctx->if_end_blk = blk;    break;
        default: break;
    }
    Ast::ctx->addLabel(id, blk);
    return blk;
}

BasicBlock* setupWhileBlock(int id) {
    auto blk = new BasicBlock(id);
    Ast::ctx->addLabel(id, blk);
    return blk;
}

void CondStmt::generateCode() {
    if (token.sym == WHILETK) {
        int while_label_id = Ast::ctx->genLabel();
        auto while_blk = setupWhileBlock(while_label_id);
        Ast::ctx->func->addBlock(while_blk);

        int body_label_id = Ast::ctx->genLabel();
        auto body_blk = setupWhileBlock(body_label_id);

        int while_end_label_id = Ast::ctx->genLabel();
        auto while_end_blk = setupWhileBlock(while_end_label_id);

        Ast::ctx->pushWhile(while_blk, body_blk, while_end_blk);
        // while condition block
        Ast::ctx->blk = while_blk;
        // TODO: implement && and ||
        condExp->generateCode();
        if (condExp->getVar()->isAddr()) {
            auto val = new Variable(Ast::ctx->func->genVar(), false, condExp->evalType());
            Ast::ctx->blk->addInst(new LoadInst(val, condExp->getVar()));
            Ast::ctx->blk->addInst(new BranchInst(Beqz, val, while_end_label_id));
        } else {
            Ast::ctx->blk->addInst(new BranchInst(Beqz, condExp->getVar(), while_end_label_id));
        }

        // while block body
        Ast::ctx->blk = body_blk;
        Ast::ctx->func->addBlock(body_blk);
        table.pushBlock();
        ifStmt->generateCode();
        table.popBlock();
        Ast::ctx->blk->addInst(new JumpInst(while_label_id));

        // while end body
        Ast::ctx->popWhile();
        Ast::ctx->func->addBlock(while_end_blk);
        Ast::ctx->blk = while_end_blk;
    } else if (token.sym == IFTK) {
        int if_label_id = Ast::ctx->genLabel();
        auto if_blk = setupIfBlock(if_label_id, IF);
        Ast::ctx->func->addBlock(if_blk);

        int body_label_id = Ast::ctx->genLabel();
        auto body_blk = setupIfBlock(body_label_id, IF_BODY);

        int else_label_id = 0, end_label_id = 0;
        BasicBlock *else_blk = nullptr, *if_end_blk = nullptr;
        if (elseStmt) {
            else_label_id = Ast::ctx->genLabel();
            else_blk = setupIfBlock(else_label_id, ELSE);
        }
        end_label_id = Ast::ctx->genLabel();
        if_end_blk = setupIfBlock(end_label_id, IF_END);

        Ast::ctx->blk = if_blk;
        condExp->generateCode();
        int next_label_id = else_blk ? else_label_id : end_label_id;
        if (condExp->getVar()->isAddr()) {
            auto val = new Variable(Ast::ctx->func->genVar(), false, condExp->evalType());
            Ast::ctx->blk->addInst(new LoadInst(val, condExp->getVar()));
            Ast::ctx->blk->addInst(new BranchInst(Beqz, val, next_label_id));
        } else {
            Ast::ctx->blk->addInst(new BranchInst(Beqz, condExp->getVar(), next_label_id));
        }
        Ast::ctx->func->addBlock(body_blk);
        Ast::ctx->blk = body_blk;
        table.pushBlock();
        ifStmt->generateCode();
        table.popBlock();
        Ast::ctx->blk->addInst(new JumpInst(end_label_id));
        if (else_blk) {
            Ast::ctx->func->addBlock(else_blk);
            Ast::ctx->blk = else_blk;
            table.pushBlock();
            elseStmt->generateCode();
            table.popBlock();
            Ast::ctx->blk->addInst(new JumpInst(end_label_id));
        }
        Ast::ctx->if_blk = nullptr;
        Ast::ctx->if_body_blk = nullptr;
        Ast::ctx->if_end_blk = nullptr;
        Ast::ctx->func->addBlock(if_end_blk);
        Ast::ctx->blk = if_end_blk;
    } else {
        // bug
    }
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

void LoopStmt::generateCode() {
    if (sym.sym == BREAKTK) {
        Ast::ctx->blk->addInst(new JumpInst(Ast::ctx->while_end_blk->getId()));
    } else if (sym.sym == CONTINUETK) {
        Ast::ctx->blk->addInst(new JumpInst(Ast::ctx->while_blk->getId()));
    } else {
        // bug
    }
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

void ReturnStmt::generateCode() {
    if (exp) {
        exp->generateCode();
        if (exp->getVar()->isAddr()) {
            auto var = new Variable(Ast::ctx->func->genVar(), false, exp->evalType());
            Ast::ctx->blk->addInst(new LoadInst(var, exp->getVar()));
            Ast::ctx->blk->addInst(new ReturnInst(var));
        } else {
            Ast::ctx->blk->addInst(new ReturnInst(exp->getVar()));
        }
    } else {
        Ast::ctx->blk->addInst(new ReturnInst());
    }
}

FormatString::FormatString() {

}

FormatString::FormatString(Symbol str) {
    sym = std::move(str);
}

Symbol FormatString::getSym() {
    return sym;
}

void FormatString::traverse(int lev) {
    cout << sym.val;
}

void FormatString::generateCode() {

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
    ctx = new Context();
    ctx->module = new Module();
    ctx->table = &table;
}

void Ast::traverse() {
    program->addError(errorHandling);
    program->traverse(0);
}

Module* Ast::generateCode() {
    program->generateCode();
    return ctx->module;
}
