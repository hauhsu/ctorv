#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Lexer.hpp"
using namespace std;

bool _DEBUG = true;
//bool _DEBUG = false;


enum Type {
  INT
};

class Variable {
public:
  Variable(const string n, Type t): name(n), type(t){;}
  string name;
  Type type;
};

class SymbolTable {
public:
  SymbolTable(shared_ptr<SymbolTable> p): prev_scop(p) {;}
  auto add(Variable var) -> void {
    symbols.insert({var.name, var});
  }

private:
    shared_ptr<SymbolTable> prev_scop;
    unordered_map<string, Variable> symbols;
};


class Function;

class Block {
  shared_ptr<Block> parent;
  SymbolTable symtable;
  shared_ptr<Function> function;
};

class Function {
public:
  string name;
  Type return_type;
  vector<Variable> params;
  shared_ptr<Block> body;
};

// Recursive decent parser
class Parser {
public:

  auto parse(const string& input) -> void {
    lexer.set_input(input);
    while(1) {
      lookahead = lexer.getNextToken();
      if (!lookahead) break;
      if (_DEBUG) {
        cout << lookahead->to_string() << endl;
      }
      if (lookahead->tag == Tag::TYPE) {
        parse_declaration();
      }
    }
  };

private:

  auto parse_declaration() -> void {
    /*
     * grammer:
     *     declaration -> type identifyer '(' parameters ')'
     */
    match(Tag::TYPE);
    identifier();
  }

  auto match(Tag t) -> void {
    if (lookahead->tag == t) {
      lookahead = lexer.getNextToken();
    } else {
      cout << "Syntax error";
    }
  }

  auto identifier() -> void {
    
  }

private:
  Lexer lexer;
  shared_ptr<Token> lookahead;
  unordered_map<string, Function> functions;
};


class Compiler {
public:
  auto compile(const string& input,
               const string& output="") -> void {
    parser.parse(input);
  }
private:
  Parser parser;
};

int main(int argc, char *argv[])
{
  auto compiler = Compiler();
  for (int i = 1; i < argc; i++) {
    compiler.compile(argv[i]);
  }

  return 0;
}
