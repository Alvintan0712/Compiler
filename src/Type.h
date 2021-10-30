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
    void addDim(int dim);
    bool operator==(const Type& t);
    bool operator!=(const Type& t);
    Symbol getType() const;
    std::vector<int> getDims() const;
private:
    Symbol type;
    std::vector<int> dims;
};


#endif //SRC_TYPE_H
