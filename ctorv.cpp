#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Parser.hpp"
using namespace std;

bool _DEBUG = true;
//bool _DEBUG = false;



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
