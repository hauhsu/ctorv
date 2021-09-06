#ifndef IR_HPP_PDHZC90X
#define IR_HPP_PDHZC90X
#include <string>
using namespace std;

/* IR opcode */
enum OP{
  OP_NOP,
  OP_JUMP,
  OP_JUMP_IF,
  OP_JUMP_IF_NOT,
  OP_CALL,
  OP_RETUREN,

  OP_LOAD,
  OP_LOAD_IMM,
  OP_STORE,

  OP_ADD,
  OP_SUBTRACT,
  OP_EQ,
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
};

auto mkIR(OP op, Addr addr0, Addr addr1, Addr addr2) -> shared_ptr<IR>;


#endif /* end of include guard: IR_HPP_PDHZC90X */
