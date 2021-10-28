#include <bits/stdc++.h>
#include "LexicalAnalyzer.h"
#include "GrammarAnalyzer.h"
#include "ErrorHandling.h"
#include "Ast.h"
using namespace std;

int main () {
    ifstream f("testfile.txt");
    f.seekg(0, f.end);
    int length = f.tellg();
    f.seekg(0, f.beg);

    string buffer(length, 0);
    f.read(&buffer[0], length);
    f.close();

    ErrorHandling* errorHandling = new ErrorHandling();
    LexicalAnalyzer lexical(buffer);
    GrammarAnalyzer grammar(lexical.getSymbols(), errorHandling);
    Ast ast(grammar.getProgram(), errorHandling);
    ast.traverse();

    return 0;
}
