#include <bits/stdc++.h>
#include "LexicalAnalyzer.h"
//#include "GrammarAnalyzer.h"
using namespace std;

int main () {
    ifstream f("testfile.txt");
    f.seekg(0, f.end);
    int length = f.tellg();
    f.seekg(0, f.beg);

    string buffer(length, 0);
    f.read(&buffer[0], length);
    f.close();

    LexicalAnalyzer lexical(buffer);
    lexical.output();
    // GrammarAnalyzer grammar(lexical.getSymbols());
    // grammar.output();

    return 0;
}
