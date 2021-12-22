//
// Created by alvin on 12/19/21.
//

#include <cmath>
#include <iostream>
#include "IrOptimizer.h"
using namespace std;
#define N 32
#define ll long long
#define ull unsigned long long

IrOptimizer::IrOptimizer() {
    this->module = nullptr;
}

IrOptimizer::IrOptimizer(Module *module) {
    this->module = module;
}

void IrOptimizer::optimize(int mode) {

}

void IrOptimizer::const_propagation() {

}
