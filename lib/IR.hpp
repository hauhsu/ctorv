#ifndef IR_HPP_PDHZC90X
#define IR_HPP_PDHZC90X
#include <string>
using namespace std;

/* IR opcode */
enum OP{
  OP_NOP,

  // branches
  OP_JUMP,    // unconditional jump
  OP_BEQ,     // ==
  OP_BNE,     // !=
  OP_BLT,     // <
  OP_BGE,     // >=
  OP_CALL,    // function call
  OP_RETURN,  // retrun from function call

  // memory access
  OP_LOAD,
  OP_STORE,

  // Arithmetic
  OP_ADD,
  OP_SUB,
  OP_MUL,

  // Compare
  OP_EQ,

  // function param
  OP_PARAM,
  OP_RETVAL,
};

typedef string Addr;
/* IR instruction */
struct IR {
    OP op;     /* IR operation */
    Addr addr0; /* Usually destination */
    Addr addr1; /* Usually first operand */
    Addr addr2; /* Usually second operand */
    IR(OP _op, Addr _addr0, Addr _addr1, Addr _addr2):
      op(_op),
      addr0(_addr0),
      addr1(_addr1),
      addr2(_addr2){;}
  string str();
  friend ostream &operator<< (ostream &os, const IR& ir);
};


#endif /* end of include guard: IR_HPP_PDHZC90X */
