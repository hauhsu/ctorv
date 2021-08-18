#ifndef PARSER_CPP_UDVXJZBP
#define PARSER_CPP_UDVXJZBP

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "AST.hpp"
using namespace std;

extern bool _DEBUG;


// Recursive decent parser
class Parser {
public:

  Parser() {
    // global symbol table
    symTables.push_back(make_shared<SymbolTable>());
  }
  auto parse(const string& input) -> void;

public:
  vector<shared_ptr<SymbolTable>> symTables;
  unordered_map<string, shared_ptr<FunctionNode>> functions;

private:
  auto parseDecl(shared_ptr<BlockNode> scope) -> void;  // add to **functions** or **symTables**
  auto parseParams() -> Params;
  auto parseBlock(shared_ptr<BlockNode> parent) -> BlockNode;
  auto parseStatm() -> void;
  auto parseIfStatm() -> void;

  auto match(Tag t) -> TokenPtr;
  auto addFunc(shared_ptr<FunctionNode>) -> void;

private:
  Lexer lexer;
  shared_ptr<Token> lookahead;
};
#endif /* end of include guard: PARSER_CPP_UDVXJZBP */
