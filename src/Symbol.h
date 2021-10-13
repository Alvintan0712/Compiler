//
// Created by tjan2 on 23/9/2021.
//

#ifndef SRC_SYMBOL_H
#define SRC_SYMBOL_H

#endif //SRC_SYMBOL_H

enum symbol {
    UNKNOWN,    // unknown symbol
    END,        // end symbol
    IDENFRR,     // Ident
    INTCONN,     // INTCONNst
    STRCONN,     // FormatString
    MIANTK,     // main
    CCONSTTK,    // const
    INTTK,      // int
    BBREAKTK,    // break
    CCONTINUETK, // continue
    IFFTK,       // if
    EELSETK,     // else
    NOTT,        // !
    ANDD,        // &&
    OR,         // ||
    WWHILETK,    // while
    GGETINTTK,   // getint
    PRINTTK,   // printf
    RETURTK,   // return
    PLUSS,       // +
    MINUS,       // -
    VOTK,     // void
    MUL,       // *
    DIVV,        // /
    MOD,        // %
    LESS,        // <
    LEQUAL,        // <=
    BGE,        // >
    GEEQ,        // >=
    EQUAL,        // ==
    NOTEQUAL,        // !=
    ASS,     // =
    SEMICOLON,     // ;
    COMM,      // ,
    LPRT,    // (
    RPRT,    // )
    LBRK,     // [
    RBRK,     // ]
    LBRE,     // {
    RBRE      // }
};