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

