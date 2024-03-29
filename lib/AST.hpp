#ifndef AST_H_L58SO2TY
#define AST_H_L58SO2TY

#include <vector>
#include <string>
#include <iostream>
#include "Lexer.hpp"

using namespace std;

typedef string Type;

class Variable {
public:
  string name;
  Type type;
  string typeLexeme;
  int initVal;

  Variable(string t, string n): type(t), name(n), typeLexeme(t) {
    ;
  }
  auto size() -> int {
    if (type == "int") return 4;
    cerr << "Unrecognized data type: " << type << endl;
    exit(1);
  }
  friend ostream &operator<< (ostream &, const Variable &);

};

class Environment {
public:
  Environment(){;}
  Environment(shared_ptr<Environment> p): prev_scop(p) {;}

  auto add(Variable var) -> void {
    symbols.insert({var.name, var});
  }

  unordered_map<string, Variable> symbols;
  auto isGlobal() -> bool {
    return prev_scop == nullptr;
  }
  shared_ptr<Environment> prev_scop;
};


struct Block {
  shared_ptr<Block> parent;  // if nullptr, it is global block
  shared_ptr<Environment> symtable;
};


typedef vector<Variable> Params;
class Function {
public:
  Function(const string&, const string&, const Params&);
  friend ostream &operator<< (ostream &, const Function &);

public:
  string name;
  Type returnType;
  Params params;
  shared_ptr<Block> body;

};

template<typename T>
void printVector(const vector<T>& v) {
  for (int i = 0; i < v.size(); i++) {
    if (i != v.size()-1) cout << v[i] << ", ";
    else cout << v[i];
  }
}



#endif /* end of include guard: AST_H_L58SO2TY */
