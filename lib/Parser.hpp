#ifndef PARSER_CPP_UDVXJZBP
#define PARSER_CPP_UDVXJZBP

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "AST.hpp"
#include "CompileUnit.hpp"
using namespace std;

extern bool _DEBUG;


// Recursive decent parser
class Parser {
public:

  Parser() {;}
  auto parse(stringstream& input) -> shared_ptr<CompileUnit>;
  auto parse(ifstream& input) -> shared_ptr<CompileUnit>;


private:
  auto parse() -> shared_ptr<CompileUnit>;
  auto parseDecl(shared_ptr<BlockNode> scope) -> void;  // add to **functions** or **symTables**
  auto parseParams() -> Params;
  auto parseBlock(shared_ptr<BlockNode> parent) -> shared_ptr<BlockNode>;
  auto parseStatm() -> void;
  auto parseIfStatm() -> void;

  auto match(Tag t) -> TokenPtr;
  auto addFunc(shared_ptr<FunctionNode>) -> void;

private:
  Lexer lexer;
  shared_ptr<Token> lookahead;
  shared_ptr<CompileUnit> cu;
};
#endif /* end of include guard: PARSER_CPP_UDVXJZBP */
