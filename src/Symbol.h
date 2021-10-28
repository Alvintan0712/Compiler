//
// Created by tjan2 on 23/9/2021.
//

#ifndef SRC_SYMBOL_H
#define SRC_SYMBOL_H
#include <string>

const std::string symbolsName[] = {
        "UNKNOWN", "END", "IDENFR", "INTCON", "STRCON", "MAINTK", "CONSTTK",
        "INTTK", "BREAKTK", "CONTINUETK", "IFTK", "ELSETK", "NOT", "AND",
        "OR", "WHILETK", "GETINTTK", "PRINTFTK", "RETURNTK", "PLUS", "MINU",
        "VOIDTK", "MULT", "DIV", "MOD", "LSS", "LEQ", "GRE", "GEQ", "EQL",
        "NEQ", "ASSIGN", "SEMICN", "COMMA", "LPARENT", "RPARENT", "LBRACK",
        "RBRACK", "LBRACE", "RBRACE"
};

enum SYMBOL {
    UNKNOWN,    // unknown symbol
    END,        // end symbol
    IDENFR,     // Ident
    INTCON,     // IntConst
    STRCON,     // FormatString
    MAINTK,     // main
    CONSTTK,    // const
    INTTK,      // int
    BREAKTK,    // break
    CONTINUETK, // continue
    IFTK,       // if
    ELSETK,     // else
    NOT,        // !
    AND,        // &&
    OR,         // ||
    WHILETK,    // while
    GETINTTK,   // getint
    PRINTFTK,   // printf
    RETURNTK,   // return
    PLUS,       // +
    MINU,       // -
    VOIDTK,     // void
    MULT,       // *
    DIV,        // /
    MOD,        // %
    LSS,        // <
    LEQ,        // <=
    GRE,        // >
    GEQ,        // >=
    EQL,        // ==
    NEQ,        // !=
    ASSIGN,     // =
    SEMICN,     // ;
    COMMA,      // ,
    LPARENT,    // (
    RPARENT,    // )
    LBRACK,     // [
    RBRACK,     // ]
    LBRACE,     // {
    RBRACE      // }
};

class Symbol {
public:
    Symbol();
    Symbol(SYMBOL symbol, std::string value, int line, int column);
    void print();

    SYMBOL sym;
    std::string val;
    int row, col;
private:
};

#endif //SRC_SYMBOL_H