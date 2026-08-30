#include "cpu/instr.h"

static void instr_execute_1op() {
    OPERAND mem;
    mem.type = OPR_MEM;
    mem.sreg = SREG_SS;
    mem.data_size = data_size;
    mem.addr = cpu.esp;
    operand_read(&mem);

    cpu.esp += data_size / 8;
    opr_src.val = mem.val;
    operand_write(&opr_src);
}

make_instr_impl_1op(pop, r, v);