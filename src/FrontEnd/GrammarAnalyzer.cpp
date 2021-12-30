//
// Created by tjan2 on 1/10/2021.
//

#include <iostream>
#include <fstream>
#include <utility>
#include "GrammarAnalyzer.h"
using namespace std;

bool isLVal;
bool inConst;
GrammarAnalyzer::GrammarAnalyzer(vector<Symbol> symbols, ErrorHandling* errorHandling) {
    src = move(symbols);
    err = errorHandling;
    table.addError(err);
    src.emplace_back(END, "", 0, 0);
    analyze();
}

void GrammarAnalyzer::analyze() {
    ptr = 0;
    sym = nextSymbol();
    program = compUnit();
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

int GrammarAnalyzer::evalInt(Exp *exp) {
    if (auto lval = dynamic_cast<LVal*>(exp)) {
        auto decl = table.findDecl(lval);
        if (!decl || !decl->isConst()) {
            return 0;
        }
        if (decl->getType().getConst() || table.isGlobal()) {
            if (lval->getDims().empty()) {
                return evalInt(decl->getInitValExp()[0]);
            } else {
                auto d = decl->getDims();
                int offset = 0;
                for (int i = 0; i < lval->getDims().size(); i++) {
                    offset = offset * d[i] + evalInt(lval->getDims()[i]);
                }
                return evalInt(decl->getInitValExp()[offset]);
            }
        }
    } else if (auto unaryExp = dynamic_cast<UnaryExp*>(exp)) {
        auto op = unaryExp->getOp();
        if (op == nullptr) {
            if (unaryExp->getSymbol().sym == INTCON) return stoi(unaryExp->getSymbol().val);
            else return evalInt(unaryExp->getExp());
        }
        if (op->sym == PLUS) {
            return evalInt(unaryExp->getExp());
        } else if (op->sym == MINU) {
            return -1 * evalInt(unaryExp->getExp());
        } else if (op->sym == NOT) {
            return !evalInt(unaryExp->getExp());
        }
    } else if (auto binaryExp = dynamic_cast<BinaryExp*>(exp)) {
        auto op = binaryExp->getOp();
        auto lhs = binaryExp->getLhs();
        auto rhs = binaryExp->getRhs();
        if (op.sym == PLUS) {
            return evalInt(lhs) + evalInt(rhs);
        } else if (op.sym == MINU) {
            return evalInt(lhs) - evalInt(rhs);
        } else if (op.sym == MULT) {
            return evalInt(lhs) * evalInt(rhs);
        } else if (op.sym == DIV) {
            return evalInt(lhs) / evalInt(rhs);
        } else if (op.sym == MOD) {
            return evalInt(lhs) % evalInt(rhs);
        } else if (op.sym == AND) {
            return evalInt(lhs) && evalInt(rhs);
        } else if (op.sym == OR) {
            return evalInt(lhs) || evalInt(rhs);
        } else if (op.sym == LSS) {
            return evalInt(lhs) < evalInt(rhs);
        } else if (op.sym == LEQ) {
            return evalInt(lhs) <= evalInt(rhs);
        } else if (op.sym == GRE) {
            return evalInt(lhs) > evalInt(rhs);
        } else if (op.sym == GEQ) {
            return evalInt(lhs) >= evalInt(rhs);
        } else if (op.sym == EQL) {
            return evalInt(lhs) == evalInt(rhs);
        } else if (op.sym == NEQ) {
            return evalInt(lhs) != evalInt(rhs);
        }
    }
    return 0;
}

Program* GrammarAnalyzer::compUnit() {
    table.pushBlock();
    vector<ProgramItem*> items;

    while (sym.sym == INTTK || sym.sym == CONSTTK) {
        if (sym.sym != CONSTTK && (viewNextSymbol().sym != IDENFR || viewNextSymbol(1).sym == LPARENT))
            break;
        vector<Decl*> v = decl();
        items.insert(items.end(), v.begin(), v.end());
    }

    while (sym.sym == INTTK || sym.sym == VOIDTK) {
        if (viewNextSymbol().sym == MAINTK)
            break;
        Func* f = funcDef();
        items.push_back(f);
        table.pushFunc(f);
    }

    if (sym.sym != INTTK) output();
    items.push_back(mainFuncDef());

    out.emplace_back("<CompUnit>");
    table.popBlock();
    return new Program(items);
}

vector<Decl*> GrammarAnalyzer::decl() {
    if (sym.sym == CONSTTK)
        return constDecl();
    else if (sym.sym == INTTK)
        return varDecl();
    else
        output();
    return {};
}

vector<Decl*> GrammarAnalyzer::constDecl() {
    vector<Decl*> decls;
    Symbol type;
    bool isConst = false;

    Symbol symbol = sym;
    if (sym.sym != CONSTTK) output();
    else {
        isConst = true;
        pushSymbol();
    }
    type = bType();

    if (sym.sym != IDENFR) output();
    else {
        auto def = constDef(isConst, type);
        decls.push_back(def);
        table.pushDecl(def);
    }

    while (sym.sym == COMMA) {
        pushSymbol();
        if (sym.sym != IDENFR) output();
        auto def = constDef(isConst, type);
        decls.push_back(def);
        table.pushDecl(def);
    }
    if (sym.sym != SEMICN) err->grammarError(symbol, SEMICN);
    else pushSymbol();

    out.emplace_back("<ConstDecl>");
    return decls;
}

Symbol GrammarAnalyzer::bType() {
    if (sym.sym == INTTK) {
        Symbol btype = sym;
        pushSymbol();
        return btype;
    } else {
        output();
    }
    return {};
}

Decl* GrammarAnalyzer::constDef(bool isConst, const Symbol& type) {
    Symbol idt;
    vector<Exp*> init;
    vector<int> dims;

    idt = ident();
    while (sym.sym == LBRACK) {
        pushSymbol();
        Symbol symbol = sym;
        dims.push_back(evalInt(constExp()));
        if (sym.sym != RBRACK) err->grammarError(symbol, RBRACK);
        else pushSymbol();
    }

    if (sym.sym != ASSIGN) output();
    else {
        pushSymbol();
        init = constInitVal();
    }

    out.emplace_back("<ConstDef>");
    return new Decl(Type(type, dims, isConst), idt, init);
}

vector<Exp*> GrammarAnalyzer::constInitVal() {
    vector<Exp*> init;

    if (sym.sym == LBRACE) {
        pushSymbol();
        if (sym.sym != RBRACE) {
            vector<Exp*> v;
            v = constInitVal();
            init.insert(init.end(), v.begin(), v.end());
            while (sym.sym == COMMA) {
                pushSymbol();
                v = constInitVal();
                init.insert(init.end(), v.begin(), v.end());
            }
        }
        if (sym.sym != RBRACE) output();
        else pushSymbol();
    } else {
        init.push_back(constExp());
    }

    out.emplace_back("<ConstInitVal>");
    return init;
}

vector<Decl*> GrammarAnalyzer::varDecl() {
    vector<Decl*> decls;
    Symbol type;

    Symbol symbol;
    type = bType();
    if (sym.sym != IDENFR) output();
    else {
        auto def = varDef(type);
        decls.push_back(def);
        table.pushDecl(def);
    }

    while (sym.sym == COMMA) {
        pushSymbol();
        if (sym.sym != IDENFR) output();
        auto def = varDef(type);
        decls.push_back(def);
        table.pushDecl(def);
    }
    if (sym.sym != SEMICN) err->grammarError(symbol, SEMICN);
    else pushSymbol();

    out.emplace_back("<VarDecl>");
    return decls;
}

Decl* GrammarAnalyzer::varDef(const Symbol& type) {
    Symbol idt;
    vector<Exp*> init;
    vector<int> dims;

    idt = ident();
    while (sym.sym == LBRACK) {
        pushSymbol();
        Symbol symbol = sym;
        dims.push_back(evalInt(constExp()));
        if (sym.sym != RBRACK) err->grammarError(symbol, RBRACK);
        else pushSymbol();
    }

    if (sym.sym == ASSIGN) {
        pushSymbol();
        init = initVal();
    }

    out.emplace_back("<VarDef>");
    return new Decl(Type(type, dims), idt, init);
}

vector<Exp*> GrammarAnalyzer::initVal() {
    vector<Exp*> init;

    if (sym.sym == LBRACE) {
        pushSymbol();
        if (sym.sym != RBRACE) {
            vector<Exp*> v;
            v = initVal();
            init.insert(init.end(), v.begin(), v.end());
            while (sym.sym == COMMA) {
                pushSymbol();
                v = initVal();
                init.insert(init.end(), v.begin(), v.end());
            }
        }
        if (sym.sym != RBRACE) output();
        else pushSymbol();
    } else {
        init.push_back(exp());
    }

    out.emplace_back("<InitVal>");
    return init;
}

Func* GrammarAnalyzer::funcDef() {
    Symbol type, idt;
    vector<int> dims;
    vector<Decl*> fParams;
    Block* blk = nullptr;

    type = funcType();
    idt = ident();

    if (sym.sym != LPARENT) output();
    Symbol symbol = sym;
    pushSymbol();
    if (sym.sym != RPARENT && sym.sym == INTTK) fParams = funcFParams();
    if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
    else pushSymbol();

    table.pushBlock();
    blk = block();
    table.popBlock();
    out.emplace_back("<FuncDef>");

    return new Func(Type(type, dims), idt, fParams, blk);
}

Func* GrammarAnalyzer::mainFuncDef() {
    Symbol type, idt;
    Block* blk = nullptr;

    if (sym.sym != INTTK) output();
    type = sym;
    pushSymbol();

    if (sym.sym != MAINTK) output();
    idt = sym;
    pushSymbol();

    if (sym.sym != LPARENT) output();
    Symbol symbol = sym;
    pushSymbol();

    if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
    else pushSymbol();
    table.pushBlock();
    blk = block();
    table.popBlock();

    out.emplace_back("<MainFuncDef>");
    return new Func(Type(type), idt, {}, blk);
}

Symbol GrammarAnalyzer::funcType() {
    if (sym.sym != INTTK && sym.sym != VOIDTK) output();
    Symbol ftype = sym;
    pushSymbol();
    out.emplace_back("<FuncType>");
    return ftype;
}

vector<Decl*> GrammarAnalyzer::funcFParams() {
    vector<Decl*> fParams;
    Decl* fParam;

    fParam = funcFParam();
    fParams.push_back(fParam);
    while (sym.sym == COMMA) {
        pushSymbol();
        fParam = funcFParam();
        fParams.push_back(fParam);
    }

    out.emplace_back("<FuncFParams>");
    return fParams;
}

Decl* GrammarAnalyzer::funcFParam() {
    Symbol type, idt;
    vector<int> dims;
    bool isConst = false, isPointer = false, isParam = true;

    type = bType();
    idt = ident();
    if (sym.sym == LBRACK) {
        Symbol symbol = sym;
        pushSymbol();
        if (sym.sym != RBRACK) err->grammarError(symbol, RBRACK);
        else pushSymbol();
        isPointer = true;

        while (sym.sym == LBRACK) {
            pushSymbol();
            symbol = sym;
            dims.push_back(evalInt(constExp()));
            if (sym.sym != RBRACK) err->grammarError(symbol, RBRACK);
            else pushSymbol();
        }
    }

    out.emplace_back("<FuncFParam>");
    return new Decl(Type(type, dims, isConst, isParam, isPointer), idt);
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

Block* GrammarAnalyzer::block() {
    Block* blk = nullptr;
    vector<BlockItem*> items;
    Symbol lBrace, rBrace;

    if (sym.sym != LBRACE) output();
    lBrace = sym;
    pushSymbol();
    while (isBlockItem()) {
        blockItem(items);
    }
    if (sym.sym != RBRACE) output();
    rBrace = sym;
    pushSymbol();

    out.emplace_back("<Block>");
    return new Block(lBrace, items, rBrace);
}

void GrammarAnalyzer::blockItem(vector<BlockItem*>& items) {
    if (sym.sym == INTTK || sym.sym == CONSTTK) {
        vector<Decl*> v = decl();
        for (auto x : v) items.push_back(x);
    } else {
        items.push_back(stmt());
    }
}

void popout(vector<string>& out) {
    out.pop_back();
    out.pop_back();
    out.pop_back();
    out.pop_back();
    out.pop_back();
}

Stmt* GrammarAnalyzer::stmt() {
    Symbol symbol = sym;
    if (sym.sym == SEMICN) {
        pushSymbol();

        out.emplace_back("<Stmt>");
        return new ExpStmt(new Exp());
    } else if (isExp()) { // [Exp];
        Exp* e;

        isLVal = false;
        e = exp();
        if (isLVal && sym.sym == ASSIGN) {
            LVal* lVal = (LVal *) e;
            Exp* assignExp;

            popout(out);
            pushSymbol();
            if (sym.sym == GETINTTK) {
                Symbol idt;

                idt = sym;
                pushSymbol();
                if (sym.sym != LPARENT) output();
                symbol = sym;
                pushSymbol();
                if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
                else pushSymbol();

                assignExp = new CallExp(idt);
            } else {
                assignExp = exp();
            }
            e = new AssignExp(lVal, assignExp);
        }
        if (sym.sym != SEMICN) err->grammarError(symbol, SEMICN);
        else pushSymbol();

        out.emplace_back("<Stmt>");
        return new ExpStmt(e);
    } else if (sym.sym == LBRACE) {
        table.pushBlock();
        Block* blk = block();
        table.popBlock();

        out.emplace_back("<Stmt>");
        return blk;
    } else if (sym.sym == IFTK) {
        Symbol tkn = sym;
        Exp* condExp = nullptr;
        Stmt *ifStmt = nullptr, *elseStmt = nullptr;

        pushSymbol();
        if (sym.sym != LPARENT) output();
        pushSymbol();

        condExp = cond();
        if (sym.sym != RPARENT) err->grammarError(tkn, RPARENT);
        else pushSymbol();

        ifStmt = stmt();
        if (sym.sym == ELSETK) {
            pushSymbol();
            elseStmt = stmt();
        }

        out.emplace_back("<Stmt>");
        return new CondStmt(tkn, condExp, ifStmt, elseStmt);
    } else if (sym.sym == WHILETK) {
        Symbol tkn = sym;
        Exp* condExp = nullptr;
        Stmt* whileStmt = nullptr;

        pushSymbol();
        if (sym.sym != LPARENT) output();
        pushSymbol();

        condExp = cond();
        if (sym.sym != RPARENT) err->grammarError(tkn, RPARENT);
        else pushSymbol();

        whileStmt = stmt();

        out.emplace_back("<Stmt>");
        return new CondStmt(tkn, condExp, whileStmt);
    } else if (sym.sym == BREAKTK || sym.sym == CONTINUETK) {
        Symbol tkn = sym;

        pushSymbol();
        if (sym.sym != SEMICN) err->grammarError(symbol, SEMICN);
        else pushSymbol();

        out.emplace_back("<Stmt>");
        return new LoopStmt(tkn);
    } else if (sym.sym == RETURNTK) {
        Symbol tkn = sym;
        Exp* returnExp = nullptr;

        pushSymbol();
        if (sym.sym != SEMICN && isExp()) returnExp = exp();
        if (sym.sym != SEMICN) err->grammarError(symbol, SEMICN);
        else pushSymbol();

        out.emplace_back("<Stmt>");
        return new ReturnStmt(tkn, returnExp);
    } else if (sym.sym == PRINTFTK) {
        Symbol idt;
        vector<Exp*> rParams;

        idt = sym;
        pushSymbol();
        if (sym.sym != LPARENT) output();
        symbol = sym;
        pushSymbol();

        rParams.push_back(new FormatString(formatString()));
        while (sym.sym == COMMA) {
            pushSymbol();
            rParams.push_back(exp());
        }

        if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
        else pushSymbol();

        if (sym.sym != SEMICN) err->grammarError(sym, SEMICN);
        else pushSymbol();

        out.emplace_back("<Stmt>");
        return new ExpStmt(new CallExp(idt, rParams));
    } else {
        output();
    }
    return nullptr;
}

Exp* GrammarAnalyzer::exp() {
    Exp* node = addExp();

    out.emplace_back("<Exp>");
    return node;
}

Exp* GrammarAnalyzer::cond() {
    Exp *e = lOrExp();
    e->addCond();

    out.emplace_back("<Cond>");
    return e;
}

UnaryExp* GrammarAnalyzer::lVal() {
    Symbol idt = ident();
    vector<Exp*> dims;

    while (sym.sym == LBRACK) {
        pushSymbol();
        Symbol symbol = sym;
        dims.push_back(exp());
        if (sym.sym != RBRACK) err->grammarError(symbol, RBRACK);
        else pushSymbol();
    }
    isLVal = true;

    out.emplace_back("<LVal>");
    auto lval = new LVal(idt, dims);
    if (inConst) {
        auto d = table.findDecl(lval);
        if (!d->isConst())
            err->constError(idt);
    }
    return lval;
}

UnaryExp* GrammarAnalyzer::primaryExp() {
    if (sym.sym == LPARENT) {
        Exp* e = nullptr;

        Symbol symbol = sym;
        pushSymbol();

        e = exp();
        if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
        else pushSymbol();

        out.emplace_back("<PrimaryExp>");
        return new UnaryExp(e);
    } else if (sym.sym == IDENFR) {
        out.emplace_back("<PrimaryExp>");
        return lVal();
    } else if (sym.sym == INTCON) {
        out.emplace_back("<PrimaryExp>");
        return number();
    } else {
        output();
    }
    return nullptr;
}

UnaryExp* GrammarAnalyzer::number() {
    Symbol num;

    if (sym.sym != INTCON) output();
    num = sym;
    pushSymbol();

    out.emplace_back("<Number>");
    return new UnaryExp(num);
}

UnaryExp* GrammarAnalyzer::unaryExp() {
    if (sym.sym == LPARENT || sym.sym == INTCON) {
        out.emplace_back("<UnaryExp>");
        return primaryExp();
    } else if (sym.sym == IDENFR || sym.sym == GETINTTK) {
        Symbol idt = sym;
        vector<Exp*> rParams;

        pushSymbol();
        if (sym.sym == LPARENT) {
            if (inConst) {
                err->constError(idt);
            }

            Symbol symbol = sym;
            pushSymbol();
            if (sym.sym != RPARENT && isExp())
                rParams = funcRParams();

            if (sym.sym != RPARENT) err->grammarError(symbol, RPARENT);
            else pushSymbol();

            out.emplace_back("<UnaryExp>");
            return new CallExp(idt, rParams);
        } else {
            out.pop_back();
            ptr -= 2;
            sym = nextSymbol();

            out.emplace_back("<UnaryExp>");
            return primaryExp();
        }
    } else if (sym.sym == PLUS || sym.sym == MINU || sym.sym == NOT) {
        Symbol op = unaryOp();
        UnaryExp* e = unaryExp();

        out.emplace_back("<UnaryExp>");
        return new UnaryExp(op, e);
    } else {
        output();
    }
    return nullptr;
}

Symbol GrammarAnalyzer::unaryOp() {
    Symbol op = sym;
    if (sym.sym == PLUS || sym.sym == MINU || sym.sym == NOT) {
        pushSymbol();
        out.emplace_back("<UnaryOp>");
    } else {
        output();
    }
    return op;
}

vector<Exp*> GrammarAnalyzer::funcRParams() {
    vector<Exp*> rParams;

    rParams.push_back(exp());
    while (sym.sym == COMMA) {
        pushSymbol();
        rParams.push_back(exp());
    }

    out.emplace_back("<FuncRParams>");
    return rParams;
}

Exp* GrammarAnalyzer::mulExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Symbol op;

    lhs = unaryExp();
    while (sym.sym == MULT || sym.sym == DIV || sym.sym == MOD) {
        op = sym;
        pushSymbol();
        rhs = unaryExp();
        out.emplace_back("<MulExp>");
        lhs = new BinaryExp(lhs, op, rhs);
    }

    out.emplace_back("<MulExp>");
    return lhs;
}

Exp* GrammarAnalyzer::addExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Symbol op;

    lhs = mulExp();
    if (isLVal && sym.sym == PLUS && viewNextSymbol().sym == PLUS) {
        auto lval = (LVal *) lhs;
        op = sym;
        pushSymbol();
        pushSymbol();
        auto exp = new BinaryExp(lval, op, new UnaryExp(Symbol(INTCON, "1", 0, 0)));
        return new AssignExp(lval, exp);
    } else if (isLVal && sym.sym == MINU && viewNextSymbol().sym == MINU) {
        auto lval = (LVal *) lhs;
        op = sym;
        pushSymbol();
        pushSymbol();
        auto exp = new BinaryExp(lval, op, new UnaryExp(Symbol(INTCON, "1", 0, 0)));
        return new AssignExp(lval, exp);
    } else {
        while (sym.sym == PLUS || sym.sym == MINU) {
            op = sym;
            pushSymbol();
            rhs = mulExp();
            out.emplace_back("<AddExp>");
            lhs = new BinaryExp(lhs, op, rhs);
        }
    }

    out.emplace_back("<AddExp>");
    return lhs;
}

Exp* GrammarAnalyzer::relExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Symbol op;

    lhs = addExp();
    while (sym.sym == LSS || sym.sym == LEQ || sym.sym == GRE || sym.sym == GEQ) {
        op = sym;
        pushSymbol();
        rhs = addExp();
        out.emplace_back("<RelExp>");
        lhs = new BinaryExp(lhs, op, rhs);
    }

    out.emplace_back("<RelExp>");
    return lhs;
}

Exp* GrammarAnalyzer::eqExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Symbol op;

    lhs = relExp();
    while (sym.sym == EQL || sym.sym == NEQ) {
        op = sym;
        pushSymbol();
        rhs = relExp();
        out.emplace_back("<EqExp>");
        lhs = new BinaryExp(lhs, op, rhs);
    }

    out.emplace_back("<EqExp>");
    return lhs;
}

Exp* GrammarAnalyzer::lAndExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Symbol op;

    lhs = eqExp();
    while (sym.sym == AND) {
        op = sym;
        pushSymbol();
        rhs = eqExp();
        out.emplace_back("<LAndExp>");
        lhs = new BinaryExp(lhs, op, rhs);
    }

    out.emplace_back("<LAndExp>");
    return lhs;
}

Exp* GrammarAnalyzer::lOrExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Symbol op;

    lhs = lAndExp();
    while (sym.sym == OR) {
        op = sym;
        pushSymbol();
        rhs = lAndExp();
        out.emplace_back("<LOrExp>");
        lhs = new BinaryExp(lhs, op, rhs);
    }

    out.emplace_back("<LOrExp>");
    return lhs;
}

Exp* GrammarAnalyzer::constExp() {
    inConst = true;
    Exp* node = addExp();
    inConst = false;

    out.emplace_back("<ConstExp>");
    return node;
}

Symbol GrammarAnalyzer::ident() {
    if (sym.sym == IDENFR) {
        Symbol s = sym;
        pushSymbol();
        return s;
    } else {
        output();
    }
    return {};
}

Symbol GrammarAnalyzer::formatString() {
    if (sym.sym == STRCON) {
        err->formatStringError(sym);
        Symbol name = sym;
        pushSymbol();
        return name;
    } else {
        output();
    }
    return {};
}

void GrammarAnalyzer::output() {
    ofstream f("output.txt");
    for (const auto& x : out) {
        f << x << endl;
    }
    exit(0);
}

Program *GrammarAnalyzer::getProgram() {
    return program;
}
