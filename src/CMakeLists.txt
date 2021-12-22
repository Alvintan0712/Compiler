cmake_minimum_required(VERSION 3.0)
project(src)

set(CMAKE_CXX_STANDARD 11)

add_executable(src main.cpp FrontEnd/LexicalAnalyzer.cpp FrontEnd/LexicalAnalyzer.h FrontEnd/Symbol.h FrontEnd/GrammarAnalyzer.cpp FrontEnd/GrammarAnalyzer.h FrontEnd/Symbol.cpp ErrorHandling.cpp ErrorHandling.h Ast.cpp Ast.h FrontEnd/Table.cpp FrontEnd/Table.h FrontEnd/Type.cpp FrontEnd/Type.h IR/IrFunc.cpp IR/IrFunc.h IR/BasicBlock.cpp IR/BasicBlock.h IR/Instruction.cpp IR/Instruction.h IR/Variable.cpp IR/Variable.h IR/Module.cpp IR/Module.h IR/Context.cpp IR/Context.h IR/IntrusiveLinkedList.h IR/IrOptimizer.h IR/IrOptimizer.cpp MIPS/Generator.cpp MIPS/Generator.h MIPS/RegManager.cpp MIPS/RegManager.h)
