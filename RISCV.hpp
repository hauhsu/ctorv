#ifndef RISCV_HPP_KJ5EB48W
#define RISCV_HPP_KJ5EB48W

#include "AST.hpp"

class Machine
{
public:
  Machine(){;}
  auto virtual emitMachineCode(SymbolTable& symbols, ostream &os) -> void = 0;
};

class RISCV: Machine
{
public:
  auto virtual emitMachineCode(SymbolTable& symbols, ostream &os) -> void;

};


#endif /* end of include guard: RISCV_HPP_KJ5EB48W */
