//
// Created by tjan2 on 17/10/2021.
//

#include "Ast.h"
using namespace std;

Decl::Decl() {
    Const = false;
    level = 0;
}

Decl::Decl(SYMBOL _bType, string _name, int _level) {
    bType = _bType;
    name = _name;
    Const = false;
    level = _level;
}

void Decl::setType(SYMBOL _bType) {
    bType = _bType;
}

SYMBOL Decl::getType() {
    return bType;
}

void Decl::setConst() {
    Const = true;
}

bool Decl::isConst() {
    return Const;
}

void Decl::addDim(int size) {
    dim.push_back(_size);
}

int Decl::getDim() {
    return dim.size();
}

std::string Decl::getName() {
    return name;
}

Func::Func(SYMBOL _returnType, std::string _name, Block _block) {
    returnType = _returnType;
    name = _name;
    block = &_block;
}

void Program::addItem(ProgramItem item) {
    program_items.push_back(&item);
}

std::vector<ProgramItem *> Program::getItems() {
    return program_items;
}

Block::Block() {
    level = 0;
}

Block::Block(int _level) {
    level = _level;
}

void Block::addItem(BlockItem item) {
    block_items.push_back(&item);
}
