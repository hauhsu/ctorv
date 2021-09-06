#include "CompileUnit.hpp"

CompileUnit::CompileUnit() {
  // global symbol table
  symbolTables.push_back(make_shared<Environment>());
}

auto CompileUnit::insertIR(shared_ptr<IR> ir) -> void {
  IRList.push_back(ir);
}
