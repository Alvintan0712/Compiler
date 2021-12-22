//
// Created by alvin on 12/19/21.
//

#ifndef SRC_IROPTIMIZER_H
#define SRC_IROPTIMIZER_H


#include "Module.h"

class IrOptimizer {
public:
    IrOptimizer();
    explicit IrOptimizer(Module* module);
    void optimize(int mode);
    void const_propagation();
private:
    Module* module;
};


#endif //SRC_IROPTIMIZER_H
