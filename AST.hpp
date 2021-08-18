#ifndef AST_H_L58SO2TY
#define AST_H_L58SO2TY

#include <vector>
#include <string>
#include <iostream>
#include "Lexer.hpp"

using namespace std;

enum Type {
  INT
};

class Variable {
public:
  string name;
  Type type;
  string typeLexeme;
  int initVal;

  Variable(string t, string n): name(n), typeLexeme(t) {
    if (t == "int") {
      type = INT;
    } else {
      cerr << "Unrecognized data type: " << t << endl;
      exit(1);
    }
  }
  auto size() -> int {
    switch (type) {
      case INT:
        return 4;
      default:
        cerr << "Unrecognized data type: " << type << endl;
        exit(1);
    }
  }
  friend ostream &operator<< (ostream &, const Variable &);

};

class SymbolTable {
public:
  SymbolTable(){;}
  SymbolTable(shared_ptr<SymbolTable> p): prev_scop(p) {;}
  auto add(Variable var) -> void {
    symbols.insert({var.name, var});
  }
  auto emitMachineCode(ofstream &os) -> void;

  unordered_map<string, Variable> symbols;
  auto isGlobal() -> bool {
    return prev_scop == nullptr;
  }
  shared_ptr<SymbolTable> prev_scop;
};

class ASTNode {
public:
  ASTNode(){;}
  auto virtual genIR(ofstream os) -> void = 0;

};


class FunctionNode;

struct BlockNode:ASTNode {
  shared_ptr<BlockNode> parent;  // if nullptr, it is global block
  shared_ptr<SymbolTable> symtable;
  shared_ptr<FunctionNode> function;
  auto virtual genIR(ofstream os) -> void{;}
};

class ExpNode: ASTNode {
  auto virtual genIR(ofstream os) -> void{;}
};

class IFNode: ASTNode {
public:
  auto genIR(ofstream os) -> void{;}

private:
  ExpNode condExp;
  BlockNode body;
};


typedef vector<Variable> Params;
class FunctionNode: ASTNode {
public:
  auto virtual genIR(ofstream os) -> void{;}
  string name;
  Type return_type;
  Params params;
  shared_ptr<BlockNode> body;
};

#endif /* end of include guard: AST_H_L58SO2TY */
