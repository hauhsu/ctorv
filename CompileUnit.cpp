#include "CompileUnit.hpp"

CompileUnit::CompileUnit() {
    symbolTables.push_back(make_shared<SymbolTable>());
}

auto CompileUnit::mkIR(OP op, int addr0, int addr1, int addr2) -> shared_ptr<IR> {
  auto ir = make_shared<IR>(op, addr0, addr1, addr2);
  IRList.push_back(ir);
  return ir;
}
