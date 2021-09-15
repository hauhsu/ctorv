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

  auto newTemp() -> string {
    return "t" + to_string(tmpCnt++);
  }
  auto newLabel() -> Addr {
    auto l = ".L" + to_string(labelCnt++);
    labelList.insert({IRList.size(), l});
    return l;
  }
  auto newLabel(const string &name) -> Addr {
    labelList.insert({IRList.size(), name});
    return name;
  }

  auto patchBranch(unsigned branchIR, Addr addr) -> void {
    IRList[branchIR]->addr2 = addr;
  }

  auto patchJump(unsigned jumpIR, Addr addr) -> void {
    IRList[jumpIR]->addr0 = addr;
  }

  auto addFunc(shared_ptr<Function>) -> void;
  vector<shared_ptr<Environment>> symbolTables;
  unordered_map<string, shared_ptr<Function>> functions;
  vector<shared_ptr<IR>> IRList;
  typedef string Label;
  unordered_map<int, Label> labelList;

  auto emitIR(OP op, Addr addr0, Addr addr1, Addr addr2) -> unsigned {
    IRList.push_back(make_shared<IR>(op, addr0, addr1, addr2));
    return IRList.size()-1;
  }

  auto emitJump(Addr target) -> unsigned {
    IRList.push_back(make_shared<IR>(OP_JUMP, target, "", ""));
    return IRList.size()-1;
  }
  auto emitReturn(Addr retValAddr="") -> unsigned {
    if (retValAddr != "") {
      IRList.push_back(make_shared<IR>(OP_RETVAL, retValAddr, "", ""));
    }
    IRList.push_back(make_shared<IR>(OP_RETURN, "", "", ""));
    return IRList.size()-1;
  }

  auto dumpIRs() -> void {
    for (int i = 0; i < IRList.size(); i++) {
      auto l = labelList.find(i);
      if (l != labelList.end()) cout << l->second << ": ";
      cout << "\t" << *IRList[i] << endl;
    }
  }

private:
  int tmpCnt;
  int labelCnt;

};

#endif /* COMPILEUNIT_H */
