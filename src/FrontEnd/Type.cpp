//
// Created by tjan2 on 29/10/2021.
//

#include "Type.h"
using namespace std;

Type::Type() {

}

Type::Type(Symbol type) {
    this->type = move(type);
    this->isConst = false;
    this->isParam = false;
    this->isPointer = false;
}

Type::Type(Symbol type, std::vector<int> dims,
           bool isConst, bool isParam, bool isPointer) {
    this->type = move(type);
    this->dims.insert(this->dims.end(), dims.begin(), dims.end());
    this->isConst = isConst;
    this->isParam = isParam;
    this->isPointer = isPointer;
}

void Type::addDim(int dim) {
    dims.push_back(dim);
}

Symbol Type::getType() const {
    return type;
}

int Type::getDim() const {
    return (int) dims.size() + isPointer;
}

vector<int> Type::getDims() const {
    return dims;
}

bool Type::getConst() const {
    return isConst;
}

bool Type::getParam() const {
    return isParam;
}

bool Type::getPointer() const {
    return isPointer;
}

bool Type::operator==(const Type& t) {
    return type == t.getType() && checkDim(t);
}

bool Type::operator!=(const Type& t) {
    return type != t.getType() || !checkDim(t);
}

bool Type::checkDim(const Type& t) const {
    if (getDim() != t.getDim()) return false;
    int n = getDim();
    if (getPointer() || t.getPointer()) {
        auto v = getDims();
        auto w = t.getDims();
        for (int i = 1; i < n; i++)
            if (v[i] != w[i])
                return false;
    } else {
        auto v = getDims();
        auto w = t.getDims();
        for (int i = 0; i < n; i++)
            if (v[i] != w[i])
                return false;
    }
    return true;
}

string Type::getString() const {
    string dim_string = isPointer ? "[]" : "";
    for (auto x : dims) dim_string += "[" + to_string(x) + "]";
    return type.val;
}

