#ifndef IR_HPP_PDHZC90X
#define IR_HPP_PDHZC90X

/* IR opcode */
enum OP{
    /* generic: intermediate use in front-end. No code generation */
    OP_generic,

    /* calling convention */
    OP_func_extry, /* function entry point */
    OP_exit,       /* program exit routine */
    OP_call,       /* function call */
    OP_indirect,   /* indirect call with function pointer */
    OP_func_exit,  /* function exit code */
    OP_return,     /* jump to function exit */

    OP_load_constant,     /* load constant */
    OP_load_data_address, /* lookup address of a constant in data section */

    /* stack operations */
    OP_push, /* push onto stack */
    OP_pop,  /* pop from stack */

    /* control flow */
    OP_jump,        /* unconditional jump */
    OP_label,       /* note label */
    OP_jz,          /* jump if false */
    OP_jnz,         /* jump if true */
    OP_block_start, /* code block start */
    OP_block_end,   /* code block end */

    /* memory address operations */
    OP_address_of, /* lookup variable's address */
    OP_read,       /* read from memory address */
    OP_write,      /* write to memory address */

    /* arithmetic operators */
    OP_add,
    OP_sub,
    OP_mul,
    OP_div,     /* signed division */
    OP_mod,     /* modulo */
    OP_ternary, /* ? : */
    OP_lshift,
    OP_rshift,
    OP_log_and,
    OP_log_or,
    OP_log_not,
    OP_eq,  /* equal */
    OP_neq, /* not equal */
    OP_lt,  /* less than */
    OP_leq, /* less than or equal */
    OP_gt,  /* greater than */
    OP_geq, /* greater than or equal */
    OP_bit_or,
    OP_bit_and,
    OP_bit_xor,
    OP_bit_not,
    OP_negate,

    /* platform-specific */
    OP_syscall,
    OP_start
};

/* IR instruction */
struct IR {
    OP op;     /* IR operation */
    int addr0; /* Usually destination */
    int addr1; /* Usually first operand */
    int addr2; /* Usually second operand */
    IR(OP _op, int _addr0, int _addr1, int _addr2):
      op(_op),
      addr0(_addr0),
      addr1(_addr1),
      addr2(_addr2){;}
};


#endif /* end of include guard: IR_HPP_PDHZC90X */
