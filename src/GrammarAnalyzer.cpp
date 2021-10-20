//
// Created by tjan2 on 1/10/2021.
//

#include <iostream>
#include <fstream>
#include "Variable.h"
#include "Function.h"
#include "GrammarAnalyzer.h"
using namespace std;

bool isLVal;
GrammarAnalyzer::GrammarAnalyzer(vector<Symbol> symbols) {
    src = symbols;
    src.push_back(Symbol(END, "", 0, 0));
    analyze();
}

void GrammarAnalyzer::analyze() {
    ptr = 0;
    sym = nextSymbol();
    _CompUnit();
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

void GrammarAnalyzer::_CompUnit() {
    while (sym.sym == INTTK || sym.sym == CONSTTK) {
        if (sym.sym != CONSTTK && (viewNextSymbol().sym != IDENFR || viewNextSymbol(1).sym == LPARENT))
            break;
        _Decl();
    }
    while (sym.sym == INTTK || sym.sym == VOIDTK) {
        if (viewNextSymbol().sym == MAINTK)
            break;
        _FuncDef();
    }
    if (sym.sym != INTTK) output();
    _MainFuncDef();
    out.push_back("<CompUnit>");
}

void GrammarAnalyzer::_Decl() {
    if (sym.sym == CONSTTK)
        _ConstDecl();
    else if (sym.sym == INTTK)
        _VarDecl();
    else
        output();
}

void GrammarAnalyzer::_ConstDecl() {
    if (sym.sym != CONSTTK) output();
    pushSymbol();
    _BType();
    if (sym.sym != IDENFR) output();
    _ConstDef();
    while (sym.sym == COMMA) {
        pushSymbol();
        if (sym.sym != IDENFR) output();
        _ConstDef();
    }
    if (sym.sym != SEMICN) output();
    pushSymbol();
    out.push_back("<ConstDecl>");
}

void GrammarAnalyzer::_BType() {
    if (sym.sym == INTTK) {
        pushSymbol();
    } else {
        output();
    }
}

void GrammarAnalyzer::_ConstDef() {
    _Ident();
    while (sym.sym == LBRACK) {
        pushSymbol();
        _ConstExp();
        if (sym.sym != RBRACK) output();
        pushSymbol();
    }
    if (sym.sym != ASSIGN) output();
    pushSymbol();
    _ConstInitVal();
    out.push_back("<ConstDef>");
}

void GrammarAnalyzer::_ConstInitVal() {
    if (sym.sym == LBRACE) {
        pushSymbol();
        if (sym.sym != RBRACE) {
            _ConstInitVal();
            while (sym.sym == COMMA) {
                pushSymbol();
                _ConstInitVal();
            }
        }
        if (sym.sym != RBRACE) output();
        pushSymbol();
    } else {
        _ConstExp();
    }
    out.push_back("<ConstInitVal>");
}

void GrammarAnalyzer::_VarDecl() {
    _BType();
    _VarDef();
    while (sym.sym == COMMA) {
        pushSymbol();
        _VarDef();
    }
    if (sym.sym != SEMICN) output();
    pushSymbol();
    out.push_back("<VarDecl>");
}

void GrammarAnalyzer::_VarDef() {
    _Ident();
    while (sym.sym == LBRACK) {
        pushSymbol();
        _ConstExp();
        if (sym.sym != RBRACK) output();
        pushSymbol();
    }
    if (sym.sym == ASSIGN) {
        pushSymbol();
        _InitVal();
    }
    out.push_back("<VarDef>");
}

void GrammarAnalyzer::_InitVal() {
    if (sym.sym == LBRACE) {
        pushSymbol();
        if (sym.sym != RBRACE) {
            _InitVal();
            while (sym.sym == COMMA) {
                pushSymbol();
                _InitVal();
            }
        }
        if (sym.sym != RBRACE) output();
        pushSymbol();
    } else {
        _Exp();
    }
    out.push_back("<InitVal>");
}

void GrammarAnalyzer::_FuncDef() {
    _FuncType();
    _Ident();
    if (sym.sym != LPARENT) output();
    pushSymbol();
    if (sym.sym != RPARENT)
        _FuncFParams();
    if (sym.sym != RPARENT) output();
    pushSymbol();
    _Block();
    out.push_back("<FuncDef>");
}

void GrammarAnalyzer::_MainFuncDef() {
    if (sym.sym != INTTK) output();
    pushSymbol();
    if (sym.sym != MAINTK) output();
    pushSymbol();
    if (sym.sym != LPARENT) output();
    pushSymbol();
    if (sym.sym != RPARENT) output();
    pushSymbol();
    _Block();
    out.push_back("<MainFuncDef>");
}

void GrammarAnalyzer::_FuncType() {
    if (sym.sym != INTTK && sym.sym != VOIDTK) output();
    pushSymbol();
    out.push_back("<FuncType>");
}

void GrammarAnalyzer::_FuncFParams() {
    _FuncFParam();
    while (sym.sym == COMMA) {
        pushSymbol();
        _FuncFParam();
    }
    out.push_back("<FuncFParams>");
}

void GrammarAnalyzer::_FuncFParam() {
    _BType();
    _Ident();
    if (sym.sym == LBRACK) {
        pushSymbol();
        if (sym.sym != RBRACK) output();
        pushSymbol();

        while (sym.sym == LBRACK) {
            pushSymbol();
            _ConstExp();
            if (sym.sym != RBRACK) output();
            pushSymbol();
        }
    }
    out.push_back("<FuncFParam>");
}

bool GrammarAnalyzer::isExp() {
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

void GrammarAnalyzer::_Block() {
    if (sym.sym != LBRACE) output();
    pushSymbol();
    while (isBlockItem())
        _BlockItem();
    if (sym.sym != RBRACE) output();
    pushSymbol();
    out.push_back("<Block>");
}

void GrammarAnalyzer::_BlockItem() {
    if (sym.sym == INTTK || sym.sym == CONSTTK) {
        _Decl();
    } else {
        _Stmt();
    }
}

void popout(vector<string>& out) {
    out.pop_back();
    out.pop_back();
    out.pop_back();
    out.pop_back();
    out.pop_back();
}

void GrammarAnalyzer::_Stmt() {
    if (sym.sym == SEMICN) {
        pushSymbol();
    } else if (isExp()) { // [Exp];
        isLVal = false;
        _Exp();
        if (isLVal && sym.sym == ASSIGN) {
            popout(out);
            pushSymbol();
            if (sym.sym == GETINTTK) {
                pushSymbol();
                if (sym.sym != LPARENT) output();
                pushSymbol();
                if (sym.sym != RPARENT) output();
                pushSymbol();
            } else {
                _Exp();
            }
        }
        if (sym.sym != SEMICN) output();
        pushSymbol();
    } else if (sym.sym == LBRACE) {
        _Block();
    } else if (sym.sym == IFTK) {
        pushSymbol();
        if (sym.sym != LPARENT) output();
        pushSymbol();
        _Cond();
        if (sym.sym != RPARENT) output();
        pushSymbol();
        _Stmt();
        if (sym.sym == ELSETK) {
            pushSymbol();
            _Stmt();
        }
    } else if (sym.sym == WHILETK) {
        pushSymbol();
        if (sym.sym != LPARENT) output();
        pushSymbol();
        _Cond();
        if (sym.sym != RPARENT) output();
        pushSymbol();
        Stmt();
    } else if (sym.sym == BREAKTK || sym.sym == CONTINUETK) {
        pushSymbol();
        if (sym.sym != SEMICN) output();
        pushSymbol();
    } else if (sym.sym == RETURNTK) {
        pushSymbol();
        if (sym.sym != SEMICN)
            _Exp();
        if (sym.sym != SEMICN) output();
        pushSymbol();
    } else if (sym.sym == PRINTFTK) {
        pushSymbol();
        if (sym.sym != LPARENT) output();
        pushSymbol();
        _FormatString();
        while (sym.sym == COMMA) {
            pushSymbol();
            _Exp();
        }
        if (sym.sym != RPARENT) output();
        pushSymbol();
        if (sym.sym != SEMICN) output();
        pushSymbol();
    } else {
        output();
    }
    out.push_back("<Stmt>");
}

void GrammarAnalyzer::_Exp() {
    _AddExp();
    out.push_back("<Exp>");
}

void GrammarAnalyzer::_Cond() {
    _LOrExp();
    out.push_back("<Cond>");
}

void GrammarAnalyzer::_LVal() {
    _Ident();
    while (sym.sym == LBRACK) {
        pushSymbol();
        _Exp();
        if (sym.sym != RBRACK) output();
        pushSymbol();
    }
    isLVal = true;
    out.push_back("<LVal>");
}

void GrammarAnalyzer::_PrimaryExp() {
    if (sym.sym == LPARENT) {
        pushSymbol();
        _Exp();
        if (sym.sym != RPARENT) output();
        pushSymbol();
    } else if (sym.sym == IDENFR) {
        _LVal();
    } else if (sym.sym == INTCON) {
        _Number();
    } else {
        output();
    }
    out.push_back("<PrimaryExp>");
}

void GrammarAnalyzer::_Number() {
    if (sym.sym != INTCON) output();
    pushSymbol();
    out.push_back("<Number>");
}

void GrammarAnalyzer::_UnaryExp() {
    if (sym.sym == LPARENT || sym.sym == INTCON)
        _PrimaryExp();
    else if (sym.sym == IDENFR) {
        pushSymbol();
        if (sym.sym == LPARENT) {
            pushSymbol();
            if (sym.sym != RPARENT)
                _FuncRParams();
            if (sym.sym != RPARENT) output();
            pushSymbol();
        } else {
            out.pop_back();
            ptr -= 2;
            sym = nextSymbol();
            _PrimaryExp();
        }
    } else if (sym.sym == PLUS || sym.sym == MINU || sym.sym == NOT) {
        _UnaryOp();
        _UnaryExp();
    } else {
        output();
    }
    out.push_back("<UnaryExp>");
}

void GrammarAnalyzer::_UnaryOp() {
    if (sym.sym == PLUS || sym.sym == MINU || sym.sym == NOT) {
        pushSymbol();
    } else {
        output();
    }
    out.push_back("<UnaryOp>");
}

void GrammarAnalyzer::_FuncRParams() {
    _Exp();
    while (sym.sym == COMMA) {
        pushSymbol();
        _Exp();
    }
    out.push_back("<FuncRParams>");
}

void GrammarAnalyzer::_MulExp() {
    _UnaryExp();
    while (sym.sym == MULT || sym.sym == DIV || sym.sym == MOD) {
        out.push_back("<MulExp>");
        pushSymbol();
        _UnaryExp();
    }
    out.push_back("<MulExp>");
}

void GrammarAnalyzer::_AddExp() {
    _MulExp();
    while (sym.sym == PLUS || sym.sym == MINU) {
        out.push_back("<AddExp>");
        pushSymbol();
        _MulExp();
    }
    out.push_back("<AddExp>");
}

void GrammarAnalyzer::_RelExp() {
    _AddExp();
    while (sym.sym == LSS || sym.sym == LEQ || sym.sym == GRE || sym.sym == GEQ) {
        out.push_back("<RelExp>");
        pushSymbol();
        _AddExp();
    }
    out.push_back("<RelExp>");
}

void GrammarAnalyzer::_EqExp() {
    _RelExp();
    while (sym.sym == EQL || sym.sym == NEQ) {
        out.push_back("<EqExp>");
        pushSymbol();
        _RelExp();
    }
    out.push_back("<EqExp>");
}

void GrammarAnalyzer::_LAndExp() {
    _EqExp();
    while (sym.sym == AND) {
        out.push_back("<LAndExp>");
        pushSymbol();
        _EqExp();
    }
    out.push_back("<LAndExp>");
}

void GrammarAnalyzer::_LOrExp() {
    _LAndExp();
    while (sym.sym == OR) {
        out.push_back("<LOrExp>");
        pushSymbol();
        _LAndExp();
    }
    out.push_back("<LOrExp>");
}

void GrammarAnalyzer::_ConstExp() {
    _AddExp();
    out.push_back("<ConstExp>");
}

void GrammarAnalyzer::_Ident() {
    if (sym.sym == IDENFR) {
        pushSymbol();
    } else {
        output();
    }
}

void GrammarAnalyzer::_FormatString() {
    if (sym.sym == STRCON) {
        pushSymbol();
    } else {
        output();
    }
}

void GrammarAnalyzer::output() {
    ofstream f("output.txt");
    for (auto x : out) {
        f << x << endl;
    }
    exit(0);
}
