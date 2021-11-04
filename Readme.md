# 2021词法分析阶段设计文档

### 编码前设计

词法分析主要目的是将整个代码解析并输出每个单词的类别。

#### 架构设计

主要架构有单词表和词法分析模块。单词表主要储存词法分析类别码的类型，词法分析模块则将代码进行解析并输出词法分析后的结果。

<img src="images/1.jpg" style="zoom:60%;" />

### 编码后设计

#### 架构设计

在解析 Ident 时有可能是关键词，因此加入了关键词列表判断该单词是否是关键词，若是则将单词归类为对应的单词类别，否则归类为 Ident。

#### 数据结构设计

使用 `map` 存储关键词表。

#### 模块

构造了语法分析器的类 LexicalAnalyzer，主要解析源码并输出词法分析的结果。词法分析模块结构如下：

```c++
class LexicalAnalyzer {
public:
    LexicalAnalyzer(string buffer);
    void initialSymbols(); // 初始化单词表
    void initialTokens();  // 初始化关键词表
    void printSrc();
    void analyze();		  // 解析源码
    void nextSymbol();	  // 解析下一个单词
    char nextChar();	  // 下一个字符
    bool checkUnary(string sym); // 检查是不是 UnaryExp 
    char viewNextChar();   // 查看下一个单词
    void output();		  // 输出词法分析结果
private:
    int ptr;	// 源码指针
    string src; // 源码
    map<string, symbol> tokens; // 关键词表
    vector<pair<symbol, string>> symbols; // 解析结果
};
```

##### nextSymbol

该函数主要功能为解析单词，若遇到单词的字符前缀或后缀相同，例如 `!`、`!=` 和 `==`、`<=`、`>=` ，将他们转换到中间状态根据下一个字符决定怎么解析该单词；否则直接解析该单词。

在解析 Ident 时需查找关键词表检验该标识符是不是关键词，若是则归类为对应的关键词，否则一律归类为标识符。

##### checkUnary

该函数是检查该字符串是不是一元符号，例如 `+` 、`-`、`*` 等字符，若是则返回 true。

### debug 经历

判别注释的逻辑写错，造成有 `/***/` 的情况会退出注释状态。

无法解析前缀为 `_` 的 Ident。

---

# 2021 语法分析阶段设计文档

将源码中根据语法进行分析，从最开始的非终结符自顶向下逐个分析所有符号。

### 编码前设计

由于要使用左递归自顶向下分析，而实验规定文法中无法进行左递归分析，所以将文法稍微进行修改

```
乘除模表达式 MulExp → UnaryExp | MulExp   ('*' | '/' | '%') UnaryExp 
转换为   => MulExp → UnaryExp {('*' | '/' | '%') UnaryExp} 

加减表达式 AddExp → MulExp | AddExp ('+' | '−') MulExp 
转换为 => AddExp → MulExp {('+' | '-') MulExp} 

关系表达式 RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
转换为 => RelExp → AddExp {('<' | '>' | '<=' | '>=') AddExp}

相等性表达式 EqExp → RelExp | EqExp ('==' | '!=') RelExp
转换为   => EqExp → RelExp {('==' | '!=') RelExp}

逻辑与表达式 LAndExp → EqExp | LAndExp '&&' EqExp
转换为   => LAndExp → LAndExp {'&&' EqExp}

逻辑或表达式 LOrExp → LAndExp | LOrExp '||' LAndExp
转换为   => LOrExp → LOrExp {'||' LAndExp}
```

一般上有 `{}` 或 `first(<文法>)` 的文法：（仅列部分文法）

```
编译单元 CompUnit → {Decl} {FuncDef} MainFuncDef 
声明 Decl → ConstDecl | VarDecl
变量定义 VarDef → Ident { '[' ConstExp ']' } | Ident { '[' ConstExp ']' } '=' InitVal
语句 Stmt → LVal '=' Exp ';' | [Exp] ';' | Block | 
		   'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 
		   'while' '(' Cond ')' Stmt | 
		   'break' ';' | 'continue' ';' | 'return' [Exp] ';' | 
		   LVal '=' 'getint''('')'';' | 
		   'printf''('FormatString{','Exp}')'';'
```

需要提前查看终结符或使用回溯，而这里设计的编译器使用的是向前看的策略解决这类问题。

### 编码后设计

语法分析设计部分中使用递归下降法进行自顶向下分析程序语法。分析结束后将结果保存在 `out` 容器内。

```cpp
class GrammarAnalyzer {
public:
    GrammarAnalyzer(vector<pair<symbol, string>> symbols);
    void analyze();
    void output();
private:
    vector<pair<symbol, string>> src;
    vector<string> out;
    pair<symbol, string> sym;

    int ptr;
    pair<symbol, string> nextSymbol();
    pair<symbol, string> viewNextSymbol();

    pair<symbol, string> viewNextSymbol(int i);
    void _CompUnit(); 		// CompUnit 非终结符			
    void _Decl();			// Decl 非终结符
    void _ConstDecl();		// ConstDecl 非终结符
    void _BType();			// BType 非终结符
    void _ConstDef();		// ConstDef 非终结符
    void _ConstInitVal();	// ConstInitVal 非终结符
    void _VarDecl();		// VarDecl 非终结符
    void _VarDef();			// VarDef 非终结符
    void _InitVal();		// InitVal 非终结符
    void _FuncDef();		// FuncDef 非终结符
    void _MainFuncDef();	// MainFuncDef 非终结符
    void _FuncType();		// FuncType 非终结符
    void _FuncFParams();	// FuncFParams 非终结符
    void _FuncFParam();		// FuncFParam 非终结符
    void _Block();			// Block 非终结符
    void _BlockItem();		// BlockItem 非终结符
    void _Stmt();			// Stmt 非终结符
    void _Exp();			// Exp 非终结符
    void _Cond();			// Cond 非终结符
    void _LVal();			// LVal 非终结符
    void _PrimaryExp();		// PrimaryExp 非终结符
    void _Number();			// Number 非终结符
    void _UnaryExp();		// UnaryExp 非终结符
    void _UnaryOp();		// UnaryOp 非终结符
    void _FuncRParams();	// FuncRParams 非终结符
    void _MulExp();			// MulExp 非终结符
    void _AddExp();			// AddExp 非终结符
    void _RelExp();			// RelExp 非终结符
    void _EqExp();			// EqExp 非终结符
    void _LAndExp();		// LAndExp 非终结符
    void _LOrExp();			// LOrExp 非终结符

    bool isExp();			// 查看当前终结符是否是 Exp
    bool isBlockItem();		// 查看当前终结符是否是 BlockItem
    void _ConstExp();		// ConstExp 非终结符
    void _Ident();			// Ident 终结符
    void pushPair();

    void FormatString();	// FormatString 终结符
};
```

由于在编码时有些地方的判断会比较复杂，因此加了两个函数 `isExp` 和 `isBlockItem` 把可能的终结符都放在该函数上。

```cpp
bool GrammarAnalyzer::isExp() {
    return sym.tkn == SEMICN  || sym.tkn == IDENFR   || sym.tkn == PLUS     ||
           sym.tkn == MINU    || sym.tkn == NOT      || sym.tkn == LPARENT  ||
           sym.tkn == INTCON;
}

bool GrammarAnalyzer::isBlockItem() {
    return sym.tkn == INTTK   || sym.tkn == CONSTTK  || isExp() ||
           sym.tkn == IFTK    || sym.tkn == WHILETK  || sym.tkn == CONTINUETK ||
           sym.tkn == BREAKTK || sym.tkn == PRINTFTK || sym.tkn == RETURNTK ||
           sym.tkn == LBRACE;
}
```

### debug 经历

在 `Stmt` 上对语法上有些理解错误

```cpp
语句 Stmt → LVal '=' Exp ';' | [Exp] ';' | Block | 
		   'if' '(' Cond ')' Stmt [ 'else' Stmt ] | 
		   'while' '(' Cond ')' Stmt | 
		   'break' ';' | 'continue' ';' | 'return' [Exp] ';' | 
		   LVal '=' 'getint''('')'';' | 
		   'printf''('FormatString{','Exp}')'';'
```

由于 `Stmt` 上 `Exp` 和 `LVal` 中有 `ident` ，之前没判断直接分析成 `LVal` 的情况，因此需要向前看有没有 `=` 在 `Stmt` 内，若有则分析到 `LVal` 即可停止，否则将 `Exp` 的分析都输出到 `output.txt` 上。

