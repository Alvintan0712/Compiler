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
    _CompUnit();
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

void GrammarAnalyzer::_CompUnit() {
    while (sym.tkn == INTTK || sym.tkn == CHARTK || sym.tkn == CONSTTK) {
        if (sym.tkn != CONSTTK && (viewNextSymbol().tkn != IDENFR || viewNextSymbol(1).tkn == LPARENT))
            break;
        _Decl();
    }
    while (sym.tkn == INTTK || sym.tkn == CHARTK || sym.tkn == VOIDTK) {
        if (viewNextSymbol().tkn == MAINTK)
            break;
        _FuncDef();
    }
    if (sym.tkn != INTTK) output();
    _MainFuncDef();
    out.push_back("<CompUnit>");
}

void GrammarAnalyzer::_Decl() {
    if (sym.tkn == CONSTTK)
        _ConstDecl();
    else if (sym.tkn == INTTK || sym.tkn == CHARTK)
        _VarDecl();
    else
        output();
}

void GrammarAnalyzer::_ConstDecl() {
    if (sym.tkn != CONSTTK) output();
    pushPair();
    sym = nextSymbol();
    _BType();
    if (sym.tkn != IDENFR) output();
    _ConstDef();
    while (sym.tkn == COMMA) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != IDENFR) output();
        _ConstDef();
    }
    if (sym.tkn != SEMICN) output();
    pushPair();
    sym = nextSymbol();
    out.push_back("<ConstDecl>");
}

void GrammarAnalyzer::_BType() {
    if (sym.tkn == INTTK) {
        pushPair();
        sym = nextSymbol();
    } else if (sym.tkn == CHARTK) {
        pushPair();
        sym = nextSymbol();
    } else {
        output();
    }
}

void GrammarAnalyzer::_ConstDef() {
    _Ident();
    while (sym.tkn == LBRACK) {
        pushPair();
        sym = nextSymbol();
        _ConstExp();
        if (sym.tkn != RBRACK) output();
        pushPair();
        sym = nextSymbol();
    }
    if (sym.tkn != ASSIGN) output();
    pushPair();
    sym = nextSymbol();
    _ConstInitVal();
    out.push_back("<ConstDef>");
}

void GrammarAnalyzer::_ConstInitVal() {
    if (sym.tkn == LBRACE) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != RBRACE) {
            _ConstInitVal();
            while (sym.tkn == COMMA) {
                pushPair();
                sym = nextSymbol();
                _ConstInitVal();
            }
        }
        if (sym.tkn != RBRACE) output();
        pushPair();
        sym = nextSymbol();
    } else {
        _ConstExp();
    }
    out.push_back("<ConstInitVal>");
}

void GrammarAnalyzer::_VarDecl() {
    _BType();
    _VarDef();
    while (sym.tkn == COMMA) {
        pushPair();
        sym = nextSymbol();
        _VarDef();
    }
    if (sym.tkn != SEMICN) output();
    pushPair();
    sym = nextSymbol();
    out.push_back("<VarDecl>");
}

void GrammarAnalyzer::_VarDef() {
    _Ident();
    while (sym.tkn == LBRACK) {
        pushPair();
        sym = nextSymbol();
        _ConstExp();
        if (sym.tkn != RBRACK) output();
        pushPair();
        sym = nextSymbol();
    }
    if (sym.tkn == ASSIGN) {
        pushPair();
        sym = nextSymbol();
        _InitVal();
    }
    out.push_back("<VarDef>");
}

void GrammarAnalyzer::_InitVal() {
    if (sym.tkn == LBRACE) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != RBRACE) {
            _InitVal();
            while (sym.tkn == COMMA) {
                pushPair();
                sym = nextSymbol();
                _InitVal();
            }
        }
        if (sym.tkn != RBRACE) output();
        pushPair();
        sym = nextSymbol();
    } else {
        _Exp();
    }
    out.push_back("<InitVal>");
}

void GrammarAnalyzer::_FuncDef() {
    _FuncType();
    _Ident();
    if (sym.tkn != LPARENT) output();
    pushPair();
    sym = nextSymbol();
    if (sym.tkn != RPARENT)
        _FuncFParams();
    if (sym.tkn != RPARENT) output();
    pushPair();
    sym = nextSymbol();
    _Block();
    out.push_back("<FuncDef>");
}

void GrammarAnalyzer::_MainFuncDef() {
    if (sym.tkn != INTTK) output();
    pushPair();
    sym = nextSymbol();
    if (sym.tkn != MAINTK) output();
    pushPair();
    sym = nextSymbol();
    if (sym.tkn != LPARENT) output();
    pushPair();
    sym = nextSymbol();
    if (sym.tkn != RPARENT) output();
    pushPair();
    sym = nextSymbol();
    _Block();
    out.push_back("<MainFuncDef>");
}

void GrammarAnalyzer::_FuncType() {
    if (sym.tkn != INTTK && sym.tkn != VOIDTK && sym.tkn != CHARTK) output();
    pushPair();
    sym = nextSymbol();
    out.push_back("<FuncType>");
}

void GrammarAnalyzer::_FuncFParams() {
    _FuncFParam();
    while (sym.tkn == COMMA) {
        pushPair();
        sym = nextSymbol();
        _FuncFParam();
    }
    out.push_back("<FuncFParams>");
}

void GrammarAnalyzer::_FuncFParam() {
    _BType();
    _Ident();
    if (sym.tkn == LBRACK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != RBRACK) output();
        pushPair();
        sym = nextSymbol();

        while (sym.tkn == LBRACK) {
            pushPair();
            sym = nextSymbol();
            _ConstExp();
            if (sym.tkn != RBRACK) output();
            pushPair();
            sym = nextSymbol();
        }
    }
    out.push_back("<FuncFParam>");
}

bool GrammarAnalyzer::isExp() {
    return sym.tkn == SEMICN  || sym.tkn == IDENFR   || sym.tkn == PLUS     ||
           sym.tkn == MINU    || sym.tkn == NOT      || sym.tkn == LPARENT  ||
           sym.tkn == INTCON  || sym.tkn == CHAR;
}

bool GrammarAnalyzer::isBlockItem() {
    return sym.tkn == INTTK   || sym.tkn == CONSTTK  || isExp() ||
           sym.tkn == IFTK    || sym.tkn == WHILETK  || sym.tkn == CONTINUETK ||
           sym.tkn == BREAKTK || sym.tkn == PRINTFTK || sym.tkn == RETURNTK ||
           sym.tkn == LBRACE  || sym.tkn == CHARTK;
}

void GrammarAnalyzer::_Block() {
    if (sym.tkn != LBRACE) output();
    pushPair();
    sym = nextSymbol();
    while (isBlockItem())
        _BlockItem();
    if (sym.tkn != RBRACE) output();
    pushPair();
    sym = nextSymbol();
    out.push_back("<Block>");
}

void GrammarAnalyzer::_BlockItem() {
    if (sym.tkn == INTTK || sym.tkn == CHARTK || sym.tkn == CONSTTK) {
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
    if (sym.tkn == SEMICN) {
        pushPair();
        sym = nextSymbol();
    } else if (isExp()) { // [_Exp];
        isLVal = false;
        _Exp();
        if (isLVal && sym.tkn == ASSIGN) {
            popout(out);
            pushPair();
            sym = nextSymbol();
            if (sym.tkn == GETINTTK) {
                pushPair();
                sym = nextSymbol();
                if (sym.tkn != LPARENT) output();
                pushPair();
                sym = nextSymbol();
                if (sym.tkn != RPARENT) output();
                pushPair();
                sym = nextSymbol();
            } else {
                _Exp();
            }
        }
        if (sym.tkn != SEMICN) output();
        pushPair();
        sym = nextSymbol();
    } else if (sym.tkn == LBRACE) {
        _Block();
    } else if (sym.tkn == IFTK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != LPARENT) output();
        pushPair();
        sym = nextSymbol();
        _Cond();
        if (sym.tkn != RPARENT) output();
        pushPair();
        sym = nextSymbol();
        _Stmt();
        if (sym.tkn == ELSETK) {
            pushPair();
            sym = nextSymbol();
            _Stmt();
        }
    } else if (sym.tkn == WHILETK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != LPARENT) output();
        pushPair();
        sym = nextSymbol();
        _Cond();
        if (sym.tkn != RPARENT) output();
        pushPair();
        sym = nextSymbol();
        _Stmt();
    } else if (sym.tkn == BREAKTK || sym.tkn == CONTINUETK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != SEMICN) output();
        pushPair();
        sym = nextSymbol();
    } else if (sym.tkn == RETURNTK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != SEMICN)
            _Exp();
        if (sym.tkn != SEMICN) output();
        pushPair();
        sym = nextSymbol();
    } else if (sym.tkn == PRINTFTK) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != LPARENT) output();
        pushPair();
        sym = nextSymbol();
        FormatString();
        while (sym.tkn == COMMA) {
            pushPair();
            sym = nextSymbol();
            _Exp();
        }
        if (sym.tkn != RPARENT) output();
        pushPair();
        sym = nextSymbol();
        if (sym.tkn != SEMICN) output();
        pushPair();
        sym = nextSymbol();
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
    while (sym.tkn == LBRACK) {
        pushPair();
        sym = nextSymbol();
        _Exp();
        if (sym.tkn != RBRACK) output();
        pushPair();
        sym = nextSymbol();
    }
    isLVal = true;
    out.push_back("<LVal>");
}

void GrammarAnalyzer::_PrimaryExp() {
    if (sym.tkn == LPARENT) {
        pushPair();
        sym = nextSymbol();
        _Exp();
        if (sym.tkn != RPARENT) output();
        pushPair();
        sym = nextSymbol();
    } else if (sym.tkn == IDENFR) {
        _LVal();
    } else if (sym.tkn == INTCON) {
        _Number();
    } else if (sym.tkn == SINGLEQUOTE) {
        _ConstChar();
    } else {
        output();
    }
    out.push_back("<PrimaryExp>");
}

void GrammarAnalyzer::_Number() {
    if (sym.tkn != INTCON) output();
    pushPair();
    sym = nextSymbol();
    out.push_back("<Number>");
}

void GrammarAnalyzer::_UnaryExp() {
    if (sym.tkn == LPARENT || sym.tkn == INTCON || sym.tkn == SINGLEQUOTE)
        _PrimaryExp();
    else if (sym.tkn == IDENFR) {
        pushPair();
        sym = nextSymbol();
        if (sym.tkn == LPARENT) {
            pushPair();
            sym = nextSymbol();
            if (sym.tkn != RPARENT)
                _FuncRParams();
            if (sym.tkn != RPARENT) output();
            pushPair();
            sym = nextSymbol();
        } else {
            out.pop_back();
            ptr -= 2;
            sym = nextSymbol();
            _PrimaryExp();
        }
    } else if (sym.tkn == PLUS || sym.tkn == MINU || sym.tkn == NOT) {
        _UnaryOp();
        _UnaryExp();
    } else {
        output();
    }
    out.push_back("<UnaryExp>");
}

void GrammarAnalyzer::_UnaryOp() {
    if (sym.tkn == PLUS || sym.tkn == MINU || sym.tkn == NOT) {
        pushPair();
        sym = nextSymbol();
    } else {
        output();
    }
    out.push_back("<UnaryOp>");
}

void GrammarAnalyzer::_FuncRParams() {
    _Exp();
    while (sym.tkn == COMMA) {
        pushPair();
        sym = nextSymbol();
        _Exp();
    }
    out.push_back("<FuncRParams>");
}

void GrammarAnalyzer::_MulExp() {
    _UnaryExp();
    while (sym.tkn == MULT || sym.tkn == DIV || sym.tkn == MOD) {
        out.push_back("<MulExp>");
        pushPair();
        sym = nextSymbol();
        _UnaryExp();
    }
    out.push_back("<MulExp>");
}

void GrammarAnalyzer::_AddExp() {
    _MulExp();
    while (sym.tkn == PLUS || sym.tkn == MINU) {
        out.push_back("<AddExp>");
        pushPair();
        sym = nextSymbol();
        _MulExp();
    }
    out.push_back("<AddExp>");
}

void GrammarAnalyzer::_RelExp() {
    _AddExp();
    while (sym.tkn == LSS || sym.tkn == LEQ || sym.tkn == GRE || sym.tkn == GEQ) {
        out.push_back("<RelExp>");
        pushPair();
        sym = nextSymbol();
        _AddExp();
    }
    out.push_back("<RelExp>");
}

void GrammarAnalyzer::_EqExp() {
    _RelExp();
    while (sym.tkn == EQL || sym.tkn == NEQ) {
        out.push_back("<EqExp>");
        pushPair();
        sym = nextSymbol();
        _RelExp();
    }
    out.push_back("<EqExp>");
}

void GrammarAnalyzer::_LAndExp() {
    _EqExp();
    while (sym.tkn == AND) {
        out.push_back("<LAndExp>");
        pushPair();
        sym = nextSymbol();
        _EqExp();
    }
    out.push_back("<LAndExp>");
}

void GrammarAnalyzer::_LOrExp() {
    _LAndExp();
    while (sym.tkn == OR) {
        out.push_back("<LOrExp>");
        pushPair();
        sym = nextSymbol();
        _LAndExp();
    }
    out.push_back("<LOrExp>");
}

void GrammarAnalyzer::_ConstExp() {
    _AddExp();
    out.push_back("<ConstExp>");
}

void GrammarAnalyzer::_Ident() {
    if (sym.tkn == IDENFR) {
        pushPair();
        sym = nextSymbol();
    } else {
        output();
    }
}

void GrammarAnalyzer::FormatString() {
    if (sym.tkn == STRCON) {
        pushPair();
        sym = nextSymbol();
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

void GrammarAnalyzer::_ConstChar() {
    if (sym.tkn != SINGLEQUOTE) output();
    pushPair();
    sym = nextSymbol();
    _NormalChar();
    if (sym.tkn != SINGLEQUOTE) output();
    pushPair();
    sym = nextSymbol();
    out.push_back("<ConstChar>");
}

void GrammarAnalyzer::_NormalChar() {
    if (sym.tkn != CHAR) output();
    pushPair();
    sym = nextSymbol();
}
