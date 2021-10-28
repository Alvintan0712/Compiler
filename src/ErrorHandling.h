//
// Created by tjan2 on 15/10/2021.
//

#ifndef SRC_ERRORHANDLING_H
#define SRC_ERRORHANDLING_H

#include "Symbol.h"
#include <vector>

enum ERROR {
    ERROR_A, // illegal symbol
    ERROR_B, // redefined Ident
    ERROR_C, // undefined Ident
    ERROR_D, // 函数参数个数不匹配
    ERROR_E, // 函数参数类型不匹配
    ERROR_F, // 无返回值的函数存在不匹配的 return 语句
    ERROR_G, // 有返回值的函数缺少 return 语句
    ERROR_H, // 不能改变常量的值
    ERROR_I, // 缺少分号
    ERROR_J, // 缺少右小括号’)’
    ERROR_K, // 缺少右中括号’]’
    ERROR_L, // printf 中格式字符与表达式个数不匹配
    ERROR_M  // 在非循环块中使用 break 和 continue 语句
};

class ErrorHandling {
public:
    ErrorHandling();
    void formatStringError(Symbol sym);
    void grammarError(Symbol sym, SYMBOL tkn);
    void pushError(int line, char err);
    void output();
private:
    std::vector<std::string> errors;
};

#endif //SRC_ERRORHANDLING_H
