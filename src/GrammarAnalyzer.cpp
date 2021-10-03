//
// Created by tjan2 on 1/10/2021.
//

#include "GrammarAnalyzer.h"
#define tkn first

bool isLVal;
GrammarAnalyzer::GrammarAnalyzer(vector<pair<symbol, string>> symbols) {
    src = symbols;
    src.push_back(make_pair(END, ""));
    analyze();
}

void GrammarAnalyzer::analyze() {
    ptr = 0;
    sym = nextSymbol();
    CompUnit();
}

void GrammarAnalyzer::pushPair() {
    out.push_back(symbolsName[sym.first] + " " + sym.second);
}

pair<symbol, string> GrammarAnalyzer::nextSymbol() {
    return src[ptr++];
}

pair<symbol, string> GrammarAnalyzer::viewNextSymbol() {
    return src[ptr];
}

pair<symbol, string> GrammarAnalyzer::viewNextSymbol(int i) {
    return src[ptr + i];
}

void GrammarAnalyzer::CompUnit() {
    while (sym.tkn == INTTK || sym.tkn == CONSTTK) {
        if (viewNextSymbol().tkn != IDENFR || viewNextSymbol(1).tkn == LPARENT)
            break;
        Decl();
    }
    while (sym.tkn == INTTK || sym.tkn == VOIDTK) {
        if (viewNextSymbol().tkn == MAINTK)
            break;
        FuncDef();
    }
    if (sym.tkn != INTTK) exit(0);
    MainFuncDef();
    out.push_back("<CompUnit>");
}

void GrammarAnalyzer::Decl() {
    if (sym.tkn == CONSTTK)
        ConstDecl();
    else if (sym.tkn == INTTK)
        VarDecl();
    else
        exit(0);
}

void GrammarAnalyzer::ConstDecl() {
    if (sym.tkn != CONSTTK) exit(0);
    pushPair();
    sym = nextSymbol();
    BType();
    if (sym.tkn != IDENFR) exit(0);
    ConstDef();
    while (sym.tkn == COMMA) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != IDENFR) exit(0);
        ConstDef();
    }
    if (sym.tkn != SEMICN) exit(0);
    pushPair();
    sym = nextSymbol();
    out.push_back("<ConstDecl>");
}

void GrammarAnalyzer::BType() {
    if (sym.tkn == INTTK) {
        pushPair();
        sym = nextSymbol();
    } else {
        exit(0);
    }
}

void GrammarAnalyzer::ConstDef() {
    Ident();
    while (sym.tkn == LBRACK) {
        pushPair();
        sym = nextSymbol();
        ConstExp();
        if (sym.tkn != RBRACK) exit(0);
        pushPair();
        sym = nextSymbol();
    }
    if (sym.tkn != ASSIGN) exit(0);
    pushPair();
    sym = nextSymbol();
    ConstInitVal();
    out.push_back("<ConstDef>");
}

void GrammarAnalyzer::ConstInitVal() {
    if (sym.tkn == LBRACE) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != RBRACE) {
            ConstInitVal();
            while (sym.tkn == COMMA) {
                pushPair();
                sym = nextSymbol();
                ConstInitVal();
            }
        }
        if (sym.tkn != RBRACE) exit(0);
        pushPair();
        sym = nextSymbol();
    } else {
        ConstExp();
    }
    out.push_back("<ConstInitVal>");
}

void GrammarAnalyzer::VarDecl() {
    BType();
    VarDef();
    while (sym.tkn == COMMA) {
        pushPair();
        sym = nextSymbol();
        VarDef();
    }
    if (sym.tkn != SEMICN) exit(0);
    pushPair();
    sym = nextSymbol();
    out.push_back("<VarDecl>");
}

void GrammarAnalyzer::VarDef() {
    Ident();
    while (sym.tkn == LBRACK) {
        pushPair();
        sym = nextSymbol();
        ConstExp();
        if (sym.tkn != RBRACK) exit(0);
        pushPair();
        sym = nextSymbol();
    }
    if (sym.tkn == ASSIGN) {
        pushPair();
        sym = nextSymbol();
        InitVal();
    }
    out.push_back("<VarDef>");
}

void GrammarAnalyzer::InitVal() {
    if (sym.tkn == LBRACE) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != RBRACE) {
            InitVal();
            while (sym.tkn == COMMA) {
                pushPair();
                sym = nextSymbol();
                InitVal();
            }
        }
        if (sym.tkn != RBRACE) exit(0);
        pushPair();
        sym = nextSymbol();
    } else {
        Exp();
    }
    out.push_back("<InitVal>");
}

void GrammarAnalyzer::FuncDef() {
    FuncType();
    Ident();
    if (sym.tkn == LPARENT) exit(0);
    pushPair();
    sym = nextSymbol();
    if (sym.tkn != RPARENT)
        FuncFParams();
    if (sym.tkn != RPARENT) exit(0);
    pushPair();
    sym = nextSymbol();
    Block();
    out.push_back("<FuncDef>");
}

void GrammarAnalyzer::MainFuncDef() {
    if (sym.tkn != INTTK) exit(0);
    pushPair();
    sym = nextSymbol();
    if (sym.tkn != MAINTK) exit(0);
    pushPair();
    sym = nextSymbol();
    if (sym.tkn != LPARENT) exit(0);
    pushPair();
    sym = nextSymbol();
    if (sym.tkn != RPARENT) exit(0);
    pushPair();
    sym = nextSymbol();
    Block();
    out.push_back("<MainFuncDef>");
}

void GrammarAnalyzer::FuncType() {
    if (sym.tkn != INTTK && sym.tkn != VOIDTK) exit(0);
    pushPair();
    sym = nextSymbol();
    out.push_back("<FuncType>");
}

void GrammarAnalyzer::FuncFParams() {
    FuncFParam();
    while (sym.tkn == COMMA) {
        pushPair();
        sym = nextSymbol();
        FuncFParam();
    }
    out.push_back("<FuncFParams>");
}

void GrammarAnalyzer::FuncFParam() {
    BType();
    Ident();
    if (sym.tkn == LBRACK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != RBRACK) exit(0);
        pushPair();
        sym = nextSymbol();

        while (sym.tkn == LBRACK) {
            pushPair();
            sym = nextSymbol();
            ConstExp();
            if (sym.tkn != RBRACK) exit(0);
            pushPair();
            sym = nextSymbol();
        }
    }
    out.push_back("<FuncFParam>");
}

bool GrammarAnalyzer::isExp() {
    return sym.tkn == SEMICN  || sym.tkn == IDENFR   || sym.tkn == PLUS     ||
           sym.tkn == MINU    || sym.tkn == NOT      || sym.tkn == LPARENT  ||
           sym.tkn == INTCON;
}

bool GrammarAnalyzer::isBlockItem() {
    return sym.tkn == INTTK   || sym.tkn == CONSTTK  || isExp() ||
           sym.tkn == IFTK    || sym.tkn == WHILETK  || sym.tkn == CONTINUETK ||
           sym.tkn == BREAKTK || sym.tkn == PRINTFTK || sym.tkn == RETURNTK;
}

void GrammarAnalyzer::Block() {
    if (sym.tkn != LBRACE) exit(0);
    pushPair();
    sym = nextSymbol();
    while (isBlockItem())
        BlockItem();
    if (sym.tkn != RBRACE) exit(0);
    pushPair();
    sym = nextSymbol();
    out.push_back("<Block>");
}

void GrammarAnalyzer::BlockItem() {
    if (sym.tkn == INTTK || sym.tkn == CONSTTK) {
        Decl();
    } else {
        Stmt();
    }
}

void popout(vector<string>& out) {
    out.pop_back();
    out.pop_back();
    out.pop_back();
    out.pop_back();
    out.pop_back();
}

void GrammarAnalyzer::Stmt() {
    if (sym.tkn == SEMICN) {
        pushPair();
        sym = nextSymbol();
    } else if (isExp()) { // [Exp];
        isLVal = false;
        Exp();
        if (isLVal && sym.tkn == ASSIGN) {
            popout(out);
            pushPair();
            sym = nextSymbol();
            if (sym.tkn == GETINTTK) {
                pushPair();
                sym = nextSymbol();
                if (sym.tkn != LPARENT) exit(0);
                pushPair();
                sym = nextSymbol();
                if (sym.tkn != RPARENT) exit(0);
                pushPair();
                sym = nextSymbol();
            } else {
                Exp();
            }
        }
        if (sym.tkn != SEMICN) exit(0);
        pushPair();
        sym = nextSymbol();
    } else if (sym.tkn == LBRACE) {
        pushPair();
        sym = nextSymbol();
        Block();
        if (sym.tkn != RBRACE) exit(0);
        pushPair();
        sym = nextSymbol();
    } else if (sym.tkn == IFTK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != LPARENT) exit(0);
        pushPair();
        sym = nextSymbol();
        Cond();
        if (sym.tkn != LPARENT) exit(0);
        pushPair();
        sym = nextSymbol();
        Stmt();
        if (sym.tkn == ELSETK) {
            pushPair();
            sym = nextSymbol();
            Stmt();
        }
    } else if (sym.tkn == WHILETK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != LPARENT) exit(0);
        pushPair();
        sym = nextSymbol();
        Cond();
        if (sym.tkn != LPARENT) exit(0);
        pushPair();
        sym = nextSymbol();
        Stmt();
    } else if (sym.tkn == BREAKTK || sym.tkn == CONTINUETK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != SEMICN) exit(0);
        pushPair();
        sym = nextSymbol();
    } else if (sym.tkn == RETURNTK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != SEMICN)
            Exp();
        if (sym.tkn != SEMICN) exit(0);
        pushPair();
        sym = nextSymbol();
    } else if (sym.tkn == PRINTFTK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != LPARENT) exit(0);
        pushPair();
        sym = nextSymbol();
        FormatString();
        while (sym.tkn == COMMA) {
            pushPair();
            sym = nextSymbol();
            Exp();
        }
        if (sym.tkn != RPARENT) exit(0);
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != SEMICN) exit(0);
        pushPair();
        sym = nextSymbol();
    } else {
        exit(0);
    }
    out.push_back("<Stmt>");
}

void GrammarAnalyzer::Exp() {
    AddExp();
    out.push_back("<Exp>");
}

void GrammarAnalyzer::Cond() {
    LOrExp();
    out.push_back("<Cond>");
}

void GrammarAnalyzer::LVal() {
    Ident();
    while (sym.tkn == LBRACK) {
        pushPair();
        sym = nextSymbol();
        Exp();
        if (sym.tkn != RBRACK) exit(0);
        pushPair();
        sym = nextSymbol();
    }
    isLVal = true;
    out.push_back("<LVal>");
}

void GrammarAnalyzer::PrimaryExp() {
    if (sym.tkn == LPARENT) {
        pushPair();
        sym = nextSymbol();
        Exp();
        if (sym.tkn == RPARENT) {
            pushPair();
            sym = nextSymbol();
        }
    } else if (sym.tkn == IDENFR) {
        LVal();
    } else if (sym.tkn == INTCON) {
        Number();
    } else {
        exit(0);
    }
    out.push_back("<PrimaryExp>");
}

void GrammarAnalyzer::Number() {
    if (sym.tkn != INTCON) exit(0);
    pushPair();
    sym = nextSymbol();
    out.push_back("<Number>");
}

void GrammarAnalyzer::UnaryExp() {
    if (sym.tkn == LPARENT || sym.tkn == INTCON)
        PrimaryExp();
    else if (sym.tkn == IDENFR) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn == LPARENT) {
            pushPair();
            sym = nextSymbol();
            FuncRParams();
            if (sym.tkn != RPARENT) exit(0);
            pushPair();
            sym = nextSymbol();
        } else {
            out.pop_back();
            ptr -= 2;
            sym = nextSymbol();
            PrimaryExp();
        }
    } else if (sym.tkn == PLUS || sym.tkn == MINU || sym.tkn == NOT) {
        UnaryOp();
        UnaryExp();
    } else {
        exit(0);
    }
    out.push_back("<UnaryExp>");
}

void GrammarAnalyzer::UnaryOp() {
    if (sym.tkn == PLUS || sym.tkn == MINU || sym.tkn == NOT) {
        pushPair();
        sym = nextSymbol();
    } else {
        exit(0);
    }
    out.push_back("<UnaryOp>");
}

void GrammarAnalyzer::FuncRParams() {
    Exp();
    while (sym.tkn == COMMA) {
        pushPair();
        sym = nextSymbol();
        Exp();
    }
    out.push_back("<FuncRParams>");
}

void GrammarAnalyzer::MulExp() {
    UnaryExp();
    while (sym.tkn == MULT || sym.tkn == DIV || sym.tkn == MOD) {
        pushPair();
        sym = nextSymbol();
        UnaryExp();
    }
    out.push_back("<MulExp>");
}

void GrammarAnalyzer::AddExp() {
    MulExp();
    while (sym.tkn == PLUS || sym.tkn == MINU) {
        pushPair();
        sym = nextSymbol();
        MulExp();
    }
    out.push_back("<AddExp>");
}

void GrammarAnalyzer::RelExp() {
    AddExp();
    while (sym.tkn == LSS || sym.tkn == LEQ || sym.tkn == GRE || sym.tkn == GEQ) {
        pushPair();
        sym = nextSymbol();
        AddExp();
    }
    out.push_back("<RelExp>");
}

void GrammarAnalyzer::EqExp() {
    RelExp();
    while (sym.tkn == EQL || sym.tkn == NEQ) {
        pushPair();
        sym = nextSymbol();
        RelExp();
    }
    out.push_back("<EqExp>");
}

void GrammarAnalyzer::LAndExp() {
    EqExp();
    while (sym.tkn == AND) {
        pushPair();
        sym = nextSymbol();
        EqExp();
    }
    out.push_back("<LAndExp>");
}

void GrammarAnalyzer::LOrExp() {
    LAndExp();
    while (sym.tkn == OR) {
        pushPair();
        sym = nextSymbol();
        LAndExp();
    }
    out.push_back("<LOrExp>");
}

void GrammarAnalyzer::ConstExp() {
    AddExp();
    out.push_back("<ConstExp>");
}

void GrammarAnalyzer::Ident() {
    if (sym.tkn == IDENFR) {
        pushPair();
        sym = nextSymbol();
    } else {
        exit(0);
    }
}

void GrammarAnalyzer::FormatString() {
    if (sym.tkn == STRCON) {
        pushPair();
        sym = nextSymbol();
    } else {
        exit(0);
    }
}

void GrammarAnalyzer::output() {
    for (auto x : out) {
        cout << x << endl;
    }
}