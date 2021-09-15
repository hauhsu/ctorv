#include <iostream>
#include "IR.hpp"

string opstr(OP op) {
  switch (op)
  {
      case OP_NOP:  return "nop";
      case OP_JUMP:  return "jump";
      case OP_BEQ:  return "beq";
      case OP_BNE:  return "bne";
      case OP_BLT:  return "blt";
      case OP_BGE:  return "bge";
      case OP_CALL:  return "call";
      case OP_RETURN:  return "return";
      case OP_LOAD:  return "load";
      case OP_STORE:  return "store";
      case OP_ADD:  return "add";
      case OP_SUB:  return "sub";
      case OP_MUL:  return "mul";
      case OP_EQ:  return "eq";
      case OP_PARAM:  return "param";
      case OP_RETVAL:  return "retval";
      default:      
        cerr << "Unknown OP: " << op << endl;
        exit(1);
  }
}

ostream &operator<< (ostream &os, const IR& ir) {

  /* // branches */
  /* OP_BEQ,     // == */
  /* OP_BNE,     // != */
  /* OP_BLT,     // < */
  /* OP_BGE,     // >= */
  /* OP_CALL,    // function call */
  /* OP_RETUREN, // retrun from function call */

  /* // memory access */
  /* OP_LOAD, */
  /* OP_STORE, */

  /* // Arithmetic */
  /* OP_ADD, */
  /* OP_SUB, */
  /* OP_MUL, */
  os << opstr(ir.op) << "\t" << ir.addr0 << ", " << ir.addr1  << ", " << ir.addr2;
  return os;
}
