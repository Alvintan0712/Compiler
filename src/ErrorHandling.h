//
// Created by tjan2 on 15/10/2021.
//

#ifndef SRC_ERRORHANDLING_H
#define SRC_ERRORHANDLING_H

#include "FrontEnd/Symbol.h"
#include <vector>

enum ERROR {
    ERROR_A, // illegal symbol
    ERROR_B, // redefined Ident
    ERROR_C, // undefined Ident
    ERROR_D, // parameters number not match
    ERROR_E, // parameter type not match
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
    void formatStringError(const Symbol &sym);
    void grammarError(const Symbol &sym, SYMBOL tkn);
    void pushError(int line, char err);
    void identRepeat(const Symbol& sym);
    void identNotFound(const Symbol& sym);
    void paramsNumNotMatch(const Symbol& sym);
    void paramsTypeNotMatch(const Symbol& sym);
    void funcNoNeedReturn(const Symbol &sym);
    void funcNeedReturn(const Symbol &sym);
    void constAssign(const Symbol& sym);
    void printfError(const Symbol &sym);
    void loopError(const Symbol &sym);
    void constError(const Symbol &sym);
    bool errorExists();
    void output();
private:
    std::vector<std::pair<int, char>> errors;
};

#endif //SRC_ERRORHANDLING_H
