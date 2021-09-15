#include "CompileUnit.hpp"

CompileUnit::CompileUnit(): tmpCnt(0), labelCnt(0) {
  // global symbol table
  symbolTables.push_back(make_shared<Environment>());
}

auto CompileUnit::insertIR(shared_ptr<IR> ir) -> void {
  IRList.push_back(ir);
}

auto CompileUnit::addFunc(shared_ptr<Function> f) -> void {
  functions.insert({f->name, f});
}

