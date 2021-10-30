//
// Created by tjan2 on 1/10/2021.
//

#include <iostream>
#include <fstream>
#include "GrammarAnalyzer.h"
using namespace std;

bool isLVal;
GrammarAnalyzer::GrammarAnalyzer(vector<Symbol> symbols, ErrorHandling* errorHandling) {
    src = symbols;
    err = errorHandling;
    src.push_back(Symbol(END, "", 0, 0));
    analyze();
}

void GrammarAnalyzer::analyze() {
    ptr = 0;
    sym = nextSymbol();
    program = _CompUnit();
    program->addError(err);
}

void GrammarAnalyzer::pushSymbol() {
    out.push_back(symbolsName[sym.sym] + " " + sym.val);
    sym = nextSymbol();
}

Symbol GrammarAnalyzer::nextSymbol() {
    return src[ptr++];
}

Symbol GrammarAnalyzer::viewNextSymbol() {
    return src[ptr];
}

Symbol GrammarAnalyzer::viewNextSymbol(int i) {
    return src[ptr + i];
}

Program* GrammarAnalyzer::_CompUnit() {
    auto* p = new Program();
    while (sym.sym == INTTK || sym.sym == CONSTTK) {
        if (sym.sym != CONSTTK && (viewNextSymbol().sym != IDENFR || viewNextSymbol(1).sym == LPARENT))
            break;
        vector<Decl*> v = _Decl();
        for (auto x : v) p->addItem(x);
    }
    while (sym.sym == INTTK || sym.sym == VOIDTK) {
        if (viewNextSymbol().sym == MAINTK)
            break;
        Func* f = _FuncDef();
        p->addItem(f);
    }
    if (sym.sym != INTTK) output();
    p->addItem(_MainFuncDef());
    out.emplace_back("<CompUnit>");
    return p;
}

vector<Decl*> GrammarAnalyzer::_Decl() {
    if (sym.sym == CONSTTK)
        return _ConstDecl();
    else if (sym.sym == INTTK)
        return _VarDecl();
    else
        output();
    return {};
}

vector<Decl*> GrammarAnalyzer::_ConstDecl() {
    vector<Decl*> v;
    Decl *node = new Decl();
    if (sym.sym != CONSTTK) output();
    Symbol symbol = sym;
    node->setConst();
    pushSymbol();
    Symbol bType = _BType();
    node->setType(bType);
    if (sym.sym != IDENFR) output();
    _ConstDef(node);
    v.push_back(node);
    while (sym.sym == COMMA) {
        node = new Decl();
        node->setConst();
        pushSymbol();
        node->setType(bType);
        if (sym.sym != IDENFR) output();
        _ConstDef(node);
        v.push_back(node);
    }
    if (sym.sym != SEMICN) err->grammarError(symbol, SEMICN);
    else pushSymbol();
    out.emplace_back("<ConstDecl>");
    return v;
}

Symbol GrammarAnalyzer::_BType() {
    if (sym.sym == INTTK) {
        Symbol btype = sym;
        pushSymbol();
        return btype;
    } else {
        output();
    }
    return Symbol();
}

void GrammarAnalyzer::_ConstDef(Decl* node) {
    node->setName(_Ident());
    while (sym.sym == LBRACK) {
        pushSymbol();
        Symbol symbol = sym;
        node->addDim(_ConstExp());
        if (sym.sym != RBRACK) err->grammarError(symbol, RBRACK);
        else pushSymbol();
    }
    if (sym.sym != ASSIGN) output();
    pushSymbol();
    _ConstInitVal(node);
    out.push_back("<ConstDef>");
}

void GrammarAnalyzer::_ConstInitVal(Decl *node) {
    if (sym.sym == LBRACE) {
        pushSymbol();
        if (sym.sym != RBRACE) {
            _ConstInitVal(node);
            while (sym.sym == COMMA) {
                pushSymbol();
                _ConstInitVal(node);
            }
        }
        if (sym.sym != RBRACE) output();
        pushSymbol();
    } else {
        node->addInitVal(_ConstExp());
    }
    out.push_back("<ConstInitVal>");
}

vector<Decl*> GrammarAnalyzer::_VarDecl() {
    vector<Decl*> v;
    Symbol symbol = sym;
    Decl* node = new Decl();
    Symbol bType = _BType();
    node->setType(bType);
    if (sym.sym != IDENFR) output();
    _VarDef(node);
    v.push_back(node);
    while (sym.sym == COMMA) {
        node = new Decl();
        node->setType(bType);
        pushSymbol();
        if (sym.sym != IDENFR) output();
        _VarDef(node);
        v.push_back(node);
    }
    if (sym.sym != SEMICN) err->grammarError(symbol, SEMICN);
    else pushSymbol();
    out.emplace_back("<VarDecl>");
    return v;
}

void GrammarAnalyzer::_VarDef(Decl* node) {
    node->setName(_Ident());
    while (sym.sym == LBRACK) {
        pushSymbol();
        Symbol symbol = sym;
        node->addDim(_ConstExp());
        if (sym.sym != RBRACK) err->grammarError(symbol, RBRACK);
        else pushSymbol();
    }
    if (sym.sym == ASSIGN) {
        pushSymbol();
        _InitVal(node);
    }
    out.emplace_back("<VarDef>");
}

void GrammarAnalyzer::_InitVal(Decl* node) {
    if (sym.sym == LBRACE) {
        pushSymbol();
        if (sym.sym != RBRACE) {
            _InitVal(node);
            while (sym.sym == COMMA) {
                pushSymbol();
                _InitVal(node);
            }
        }
        if (sym.sym != RBRACE) output();
        pushSymbol();
    } else {
        node->addInitVal(_Exp());
    }
    out.push_back("<InitVal>");
}

Func* GrammarAnalyzer::_FuncDef() {
    Symbol bType = _FuncType();
    Symbol ident = _Ident();
    if (sym.sym != LPARENT) output();
    Symbol symbol = sym;
    pushSymbol();
    vector<Decl*> v;
    if (sym.sym != RPARENT) v = _FuncFParams();
    if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
    else pushSymbol();
    Block* blk = _Block();
    out.emplace_back("<FuncDef>");
    return new Func(bType, ident, blk, v);
}

Func* GrammarAnalyzer::_MainFuncDef() {
    if (sym.sym != INTTK) output();
    Symbol bType = sym;
    pushSymbol();
    if (sym.sym != MAINTK) output();
    Symbol ident = sym;
    pushSymbol();
    if (sym.sym != LPARENT) output();
    Symbol symbol = sym;
    pushSymbol();
    if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
    else pushSymbol();
    Block* blk = _Block();
    out.emplace_back("<MainFuncDef>");
    return new Func(bType, ident, blk);
}

Symbol GrammarAnalyzer::_FuncType() {
    if (sym.sym != INTTK && sym.sym != VOIDTK) output();
    Symbol ftype = sym;
    pushSymbol();
    out.emplace_back("<FuncType>");
    return ftype;
}

vector<Decl*> GrammarAnalyzer::_FuncFParams() {
    vector<Decl*> v;
    Decl* fparam = _FuncFParam();
    fparam->addParam();
    v.push_back(fparam);
    while (sym.sym == COMMA) {
        pushSymbol();
        fparam = _FuncFParam();
        fparam->addParam();
        v.push_back(fparam);
    }
    out.emplace_back("<FuncFParams>");
    return v;
}

Decl* GrammarAnalyzer::_FuncFParam() {
    Symbol btype = _BType();
    Symbol ident = _Ident();
    Decl* fparam = new Decl(btype, ident);
    if (sym.sym == LBRACK) {
        Symbol symbol = sym;
        pushSymbol();
        if (sym.sym != RBRACK) err->grammarError(symbol, RBRACK);
        else pushSymbol();
        fparam->addDim();
        while (sym.sym == LBRACK) {
            pushSymbol();
            symbol = sym;
            Exp* exp = _ConstExp();
            if (sym.sym != RBRACK) err->grammarError(symbol, RBRACK);
            else pushSymbol();
            fparam->addDim(exp);
        }
    }
    out.emplace_back("<FuncFParam>");
    return fparam;
}

bool GrammarAnalyzer::isExp() const {
    return sym.sym == SEMICN  || sym.sym == IDENFR   || sym.sym == PLUS     ||
           sym.sym == MINU    || sym.sym == NOT      || sym.sym == LPARENT  ||
           sym.sym == INTCON;
}

bool GrammarAnalyzer::isBlockItem() {
    return sym.sym == INTTK   || sym.sym == CONSTTK  || isExp() ||
           sym.sym == IFTK    || sym.sym == WHILETK  || sym.sym == CONTINUETK ||
           sym.sym == BREAKTK || sym.sym == PRINTFTK || sym.sym == RETURNTK ||
           sym.sym == LBRACE;
}

Block* GrammarAnalyzer::_Block() {
    auto* blk = new Block();
    if (sym.sym != LBRACE) output();
    blk->setLBrace(sym);
    pushSymbol();
    while (isBlockItem()) {
        _BlockItem(blk);
    }
    if (sym.sym != RBRACE) output();
    blk->setRBrace(sym);
    pushSymbol();
    out.emplace_back("<Block>");
    return blk;
}

void GrammarAnalyzer::_BlockItem(Block* blk) {
    if (sym.sym == INTTK || sym.sym == CONSTTK) {
        vector<Decl*> v = _Decl();
        for (auto x : v) blk->addItem(x);
    } else {
        blk->addItem(_Stmt());
    }
}

void popout(vector<string>& out) {
    out.pop_back();
    out.pop_back();
    out.pop_back();
    out.pop_back();
    out.pop_back();
}

Stmt* GrammarAnalyzer::_Stmt() {
    Symbol symbol = sym;
    if (sym.sym == SEMICN) {
        pushSymbol();
        out.emplace_back("<Stmt>");
        auto* stmt = new ExpStmt();
        stmt->addExp(new Exp());
        return stmt;
    } else if (isExp()) { // [Exp];
        auto* expStmt = new ExpStmt();
        isLVal = false;
        Exp* exp = _Exp();
        if (isLVal && sym.sym == ASSIGN) {
            LVal* left = (LVal *) exp;
            Exp* right;
            popout(out);
            pushSymbol();
            if (sym.sym == GETINTTK) {
                right = new CallExp(sym);
                pushSymbol();
                if (sym.sym != LPARENT) output();
                symbol = sym;
                pushSymbol();
                if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
                else pushSymbol();
            } else {
                right = _Exp();
            }
            exp = new AssignExp(left, right);
        }
        expStmt->addExp(exp);
        if (sym.sym != SEMICN) err->grammarError(symbol, SEMICN);
        else pushSymbol();
        out.emplace_back("<Stmt>");
        return expStmt;
    } else if (sym.sym == LBRACE) {
        Block* blk = _Block();
        out.emplace_back("<Stmt>");
        return blk;
    } else if (sym.sym == IFTK) {
        Symbol name = sym;
        pushSymbol();
        if (sym.sym != LPARENT) output();
        pushSymbol();
        Exp* e = _Cond();
        if (sym.sym != RPARENT) err->grammarError(name, RPARENT);
        else pushSymbol();
        Stmt* ifStmt = _Stmt();
        auto* condStmt = new CondStmt(name, e, ifStmt);
        if (sym.sym == ELSETK) {
            pushSymbol();
             condStmt->addElse(_Stmt());
        }
        out.emplace_back("<Stmt>");
        return condStmt;
    } else if (sym.sym == WHILETK) {
        Symbol name = sym;
        pushSymbol();
        if (sym.sym != LPARENT) output();
        pushSymbol();
        Exp* e = _Cond();
        if (sym.sym != RPARENT) err->grammarError(name, RPARENT);
        else pushSymbol();
        Stmt* whileStmt = _Stmt();
        auto* condStmt = new CondStmt(name, e, whileStmt);
        out.emplace_back("<Stmt>");
        return condStmt;
    } else if (sym.sym == BREAKTK || sym.sym == CONTINUETK) {
        Symbol name = sym;
        pushSymbol();
        if (sym.sym != SEMICN) err->grammarError(symbol, SEMICN);
        else pushSymbol();
        out.emplace_back("<Stmt>");
        return new LoopStmt(name);
    } else if (sym.sym == RETURNTK) {
        auto* returnStmt = new ReturnStmt(sym);
        pushSymbol();
        if (sym.sym != SEMICN && isExp()) returnStmt->addExp(_Exp());
        if (sym.sym != SEMICN) err->grammarError(symbol, SEMICN);
        else pushSymbol();
        out.emplace_back("<Stmt>");
        return returnStmt;
    } else if (sym.sym == PRINTFTK) {
        auto* expStmt = new ExpStmt();
        auto* exp = new CallExp(sym);
        pushSymbol();
        if (sym.sym != LPARENT) output();
        symbol = sym;
        pushSymbol();
        auto* formatString = new FormatString(_FormatString());
        exp->addParam(formatString);
        while (sym.sym == COMMA) {
            pushSymbol();
            exp->addParam(_Exp());
        }
        if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
        else pushSymbol();
        if (sym.sym != SEMICN) err->grammarError(sym, SEMICN);
        else pushSymbol();
        out.emplace_back("<Stmt>");
        expStmt->addExp(exp);
        return expStmt;
    } else {
        output();
    }
    return nullptr;
}

Exp* GrammarAnalyzer::_Exp() {
    Exp* node = _AddExp();
    out.emplace_back("<Exp>");
    return node;
}

Exp* GrammarAnalyzer::_Cond() {
    Exp *e = _LOrExp();
    e->addCond();
    out.emplace_back("<Cond>");
    return e;
}

UnaryExp* GrammarAnalyzer::_LVal() {
    Symbol ident = _Ident();
    LVal* node = new LVal(ident);
    while (sym.sym == LBRACK) {
        pushSymbol();
        Symbol symbol = sym;
        node->addDim(_Exp());
        if (sym.sym != RBRACK) err->grammarError(symbol, RBRACK);
        else pushSymbol();
    }
    isLVal = true;
    out.emplace_back("<LVal>");
    return node;
}

UnaryExp* GrammarAnalyzer::_PrimaryExp() {
    if (sym.sym == LPARENT) {
        Symbol symbol = sym;
        pushSymbol();
        auto* node = new UnaryExp();
        node->setExp(_Exp());
        if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
        else pushSymbol();
        out.emplace_back("<PrimaryExp>");
        return node;
    } else if (sym.sym == IDENFR) {
        out.emplace_back("<PrimaryExp>");
        return _LVal();
    } else if (sym.sym == INTCON) {
        out.emplace_back("<PrimaryExp>");
        return _Number();
    } else {
        output();
    }
    return nullptr;
}

UnaryExp* GrammarAnalyzer::_Number() {
    if (sym.sym != INTCON) output();
    auto* num = new UnaryExp();
    num->addSym(sym);
    pushSymbol();
    out.emplace_back("<Number>");
    return num;
}

UnaryExp* GrammarAnalyzer::_UnaryExp() {
    if (sym.sym == LPARENT || sym.sym == INTCON) {
        out.emplace_back("<UnaryExp>");
        return _PrimaryExp();
    } else if (sym.sym == IDENFR) {
        Symbol ident = sym;
        pushSymbol();
        if (sym.sym == LPARENT) {
            Symbol symbol = sym;
            auto* f = new CallExp(ident);
            pushSymbol();
            if (sym.sym != RPARENT) {
                vector<Exp*> v = _FuncRParams();
                for (auto x : v) f->addParam(x);
            }
            if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
            else pushSymbol();
            out.emplace_back("<UnaryExp>");
            return f;
        } else {
            out.pop_back();
            ptr -= 2;
            sym = nextSymbol();
            out.emplace_back("<UnaryExp>");
            return _PrimaryExp();
        }
    } else if (sym.sym == PLUS || sym.sym == MINU || sym.sym == NOT) {
        Symbol op = _UnaryOp();
        UnaryExp* node = _UnaryExp();
        node->addOp(op.val);
        out.emplace_back("<UnaryExp>");
        return node;
    } else {
        output();
    }
    return nullptr;
}

Symbol GrammarAnalyzer::_UnaryOp() {
    Symbol op = sym;
    if (sym.sym == PLUS || sym.sym == MINU || sym.sym == NOT) {
        pushSymbol();
        out.emplace_back("<UnaryOp>");
    } else {
        output();
    }
    return op;
}

vector<Exp*> GrammarAnalyzer::_FuncRParams() {
    vector<Exp*> v;
    v.push_back(_Exp());
    while (sym.sym == COMMA) {
        pushSymbol();
        v.push_back(_Exp());
    }
    out.emplace_back("<FuncRParams>");
    return v;
}

Exp* GrammarAnalyzer::_MulExp() {
    Exp* left = _UnaryExp();
    while (sym.sym == MULT || sym.sym == DIV || sym.sym == MOD) {
        auto* op = new BinaryExp();
        op->setLhs(left);
        op->setVal(sym);
        out.emplace_back("<MulExp>");
        pushSymbol();
        op->setRhs(_UnaryExp());
        left = op;
    }
    out.emplace_back("<MulExp>");
    return left;
}

Exp* GrammarAnalyzer::_AddExp() {
    Exp* left = _MulExp();
    while (sym.sym == PLUS || sym.sym == MINU) {
        auto* op = new BinaryExp();
        op->setLhs(left);
        op->setVal(sym);
        out.emplace_back("<AddExp>");
        pushSymbol();
        op->setRhs(_MulExp());
        left = op;
    }
    out.emplace_back("<AddExp>");
    return left;
}

Exp* GrammarAnalyzer::_RelExp() {
    Exp* left = _AddExp();
    while (sym.sym == LSS || sym.sym == LEQ || sym.sym == GRE || sym.sym == GEQ) {
        auto* op = new BinaryExp();
        op->setLhs(left);
        op->setVal(sym);
        out.emplace_back("<RelExp>");
        pushSymbol();
        op->setRhs(_AddExp());
        left = op;
    }
    out.emplace_back("<RelExp>");
    return left;
}

Exp* GrammarAnalyzer::_EqExp() {
    Exp* left = _RelExp();
    while (sym.sym == EQL || sym.sym == NEQ) {
        auto* op = new BinaryExp();
        op->setLhs(left);
        op->setVal(sym);
        out.emplace_back("<EqExp>");
        pushSymbol();
        op->setRhs(_RelExp());
        left = op;
    }
    out.emplace_back("<EqExp>");
    return left;
}

Exp* GrammarAnalyzer::_LAndExp() {
    Exp* left = _EqExp();
    while (sym.sym == AND) {
        auto* op = new BinaryExp();
        op->setLhs(left);
        op->setVal(sym);
        out.emplace_back("<LAndExp>");
        pushSymbol();
        op->setRhs(_EqExp());
        left = op;
    }
    out.emplace_back("<LAndExp>");
    return left;
}

Exp* GrammarAnalyzer::_LOrExp() {
    Exp* left = _LAndExp();
    while (sym.sym == OR) {
        auto* op = new BinaryExp();
        op->setLhs(left);
        op->setVal(sym);
        out.emplace_back("<LOrExp>");
        pushSymbol();
        op->setRhs(_LAndExp());
        left = op;
    }
    out.emplace_back("<LOrExp>");
    return left;
}

Exp* GrammarAnalyzer::_ConstExp() {
    Exp* node = _AddExp();
    out.push_back("<ConstExp>");
    return node;
}

Symbol GrammarAnalyzer::_Ident() {
    if (sym.sym == IDENFR) {
        Symbol s = sym;
        pushSymbol();
        return s;
    } else {
        output();
    }
    return Symbol();
}

Symbol GrammarAnalyzer::_FormatString() {
    if (sym.sym == STRCON) {
        err->formatStringError(sym);
        Symbol name = sym;
        pushSymbol();
        return name;
    } else {
        output();
    }
    return Symbol();
}

void GrammarAnalyzer::output() {
    ofstream f("output.txt");
    for (auto x : out) {
        f << x << endl;
    }
    exit(0);
}

Program *GrammarAnalyzer::getProgram() {
    return program;
}
