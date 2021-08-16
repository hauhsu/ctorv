#ifndef PARSER_CPP_UDVXJZBP
#define PARSER_CPP_UDVXJZBP

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "Lexer.hpp"
using namespace std;

extern bool _DEBUG;

enum Type {
  INT
};

class Variable {
public:
  Variable(Id n, Word t): name(n), type(t){;}
  Id name;
  Word type;
  int initVal;
  friend ostream &operator<< (ostream &, const Variable &);
};

class SymbolTable {
public:
  SymbolTable(shared_ptr<SymbolTable> p): prev_scop(p) {;}
  auto add(Variable var) -> void {
    symbols.insert({var.name.lexeme, var});
  }

private:
    shared_ptr<SymbolTable> prev_scop;
    unordered_map<string, Variable> symbols;
};


class Function;

class Block {
public:
  shared_ptr<Block> parent;
  SymbolTable symtable;
  shared_ptr<Function> function;
};

typedef vector<Variable> Params;
class Function {
public:
  string name;
  Type return_type;
  Params params;
  shared_ptr<Block> body;
};

// Recursive decent parser
class Parser {
public:

  auto parse(const string& input) -> void;

private:

  auto parseDecl() -> void;  // add to **functions** or **symtable**
  auto parseParams() -> Params;
  auto parseBlock() -> Block;
  auto parseStatm() -> void;
  auto parseIfStatm() -> void;

  auto match(Tag t) -> TokenPtr;
  auto addFunc(shared_ptr<Function>) -> void;

private:
  Lexer lexer;
  shared_ptr<Token> lookahead;
  unordered_map<string, shared_ptr<Function>> functions;
};
#endif /* end of include guard: PARSER_CPP_UDVXJZBP */
