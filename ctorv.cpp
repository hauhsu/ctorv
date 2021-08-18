#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Parser.hpp"
#include "RISCV.hpp"
#include "AST.hpp"
using namespace std;

bool _DEBUG = true;
//bool _DEBUG = false;



class Compiler {
public:
  auto compile(const string& input,
               const string& output="") -> void {
    parser.parse(input);
    rv.emitMachineCode(*parser.symTables[0], cout);
  }
private:
  Parser parser;
  RISCV rv;
};

int main(int argc, char *argv[])
{
  auto compiler = Compiler();
  for (int i = 1; i < argc; i++) {
    compiler.compile(argv[i]);
  }

  return 0;
}
