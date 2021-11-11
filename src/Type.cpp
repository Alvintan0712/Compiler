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
    return dims.size() + isPointer;
}

vector<int> Type::getDims() const {
    return dims;
}

bool Type::getConst() {
    return isConst;
}

bool Type::getParam() {
    return isParam;
}

bool Type::getPointer() {
    return isPointer;
}

bool Type::operator==(const Type& t) {
    return type == t.getType() && dims.size() == t.getDims().size();
}

bool Type::operator!=(const Type& t) {
    return type != t.getType() || getDim() != t.getDim();
}

