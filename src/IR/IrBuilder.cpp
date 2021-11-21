//
// Created by tjan2 on 17/11/2021.
//

#include "IrBuilder.h"
using namespace std;

IrBuilder::IrBuilder() {
    label_id = 1;
    global_id = 1;
}

int IrBuilder::genGlobal() {
    return global_id++;
}

int IrBuilder::genLabel() {
    return label_id++;
}


