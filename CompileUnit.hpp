#ifndef COMPILEUNIT_H
#define COMPILEUNIT_H

#include <vector>
#include "IR.hpp"
#include "AST.hpp"
using namespace std;

class CompileUnit
{
private:
  vector<shared_ptr<IR>> IRList;

public:
  CompileUnit();
  virtual ~CompileUnit(){;}

  auto mkIR(OP op, int addr0, int addr1, int addr3) -> shared_ptr<IR>;

  vector<shared_ptr<SymbolTable>> symbolTables;
  unordered_map<string, shared_ptr<FunctionNode>> functions;

};

#endif /* COMPILEUNIT_H */
