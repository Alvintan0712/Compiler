//
// Created by tjan2 on 29/10/2021.
//

#include "Type.h"

Type::Type() {

}

Type::Type(Symbol type) {
    this->type = type;
}

void Type::addDim(int dim) {
    dims.push_back(dim);
}

Symbol Type::getType() const {
    return type;
}

std::vector<int> Type::getDims() const {
    return dims;
}

bool Type::operator==(const Type& t) {
    return type == t.getType() && dims.size() == t.getDims().size();
}

bool Type::operator!=(const Type& t) {
    return type != t.getType() || dims.size() != t.getDims().size();
}

