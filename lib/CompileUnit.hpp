#ifndef COMPILEUNIT_H
#define COMPILEUNIT_H

#include <vector>
#include "IR.hpp"
#include "AST.hpp"
using namespace std;

class CompileUnit
{
public:
  CompileUnit();
  virtual ~CompileUnit(){;}

  auto insertIR(shared_ptr<IR> ir) -> void;

  vector<shared_ptr<Environment>> symbolTables;
  unordered_map<string, shared_ptr<FunctionNode>> functions;
  vector<shared_ptr<IR>> IRList;

};

#endif /* COMPILEUNIT_H */
