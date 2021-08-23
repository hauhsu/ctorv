#include "RISCV.hpp"

auto RISCV::emitMachineCode(SymbolTable& symTable, ostream &os) -> void {
  cout << "Emit machine code" << endl;
  if (symTable.isGlobal()) {
    for (auto& s: symTable.symbols) {
      // currently put all var in .data section (no .bss nor .sdata)
      os << "\t.global " << s.second.name << endl;
      os << "\t.section .data,\"aw\"" << endl;
      os << "\t.align 2" << endl;
      os << "\t.type " << s.second.name << " @object" << endl;
      os << "\t.size " << s.second.name << " " << s.second.size() << endl;
      os << s.second.name << ":" << endl;
      os << "\t.word " << s.second.initVal << endl;
    }
  }
}
