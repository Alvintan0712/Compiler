//
// Created by tjan2 on 29/10/2021.
//

#ifndef SRC_TYPE_H
#define SRC_TYPE_H


#include <vector>
#include "Symbol.h"

class Type {
public:
    Type();
    explicit Type(Symbol type);
    explicit Type(Symbol type, std::vector<int> dims,
                  bool isConst = false, bool isParam = false, bool isPointer = false);

    bool operator==(const Type& t);
    bool operator!=(const Type& t);

    bool getConst();
    bool getParam();
    bool getPointer() const;
    int getDim() const;
    Symbol getType() const;
    std::vector<int> getDims() const;
    std::string getString() const;

    void addDim(int dim);
private:
    Symbol type;
    bool isConst, isParam, isPointer;
    std::vector<int> dims;

    bool checkDim(const Type& t);
};


#endif //SRC_TYPE_H
