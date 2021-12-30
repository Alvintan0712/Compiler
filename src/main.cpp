#include <bits/stdc++.h>
#include "FrontEnd/LexicalAnalyzer.h"
#include "FrontEnd/GrammarAnalyzer.h"
#include "ErrorHandling.h"
#include "Ast.h"
#include "MIPS/Generator.h"
#include "IR/IrOptimizer.h"

using namespace std;

int main () {
    ifstream f("testfile.txt");
    f.seekg(0, f.end);
    int length = f.tellg();
    f.seekg(0, f.beg);

    int optim = 0, useReg = 0;

    string buffer(length, 0);
    f.read(&buffer[0], length);
    f.close();
    auto* errorHandling = new ErrorHandling();
    LexicalAnalyzer lexical(buffer);
    GrammarAnalyzer grammar(lexical.getSymbols(), errorHandling);
    Ast ast(grammar.getProgram(), errorHandling);
    ast.traverse();
    if (errorHandling->errorExists()) {
        errorHandling->output();
    }
    else {
        auto module = ast.generateCode();
        module->show();
        Generator g(module, optim, useReg);
        g.show();
    }

    return 0;
}
