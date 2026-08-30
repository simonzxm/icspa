#include "cpu/instr.h"

static void instr_execute_1op() {
    operand_read(&opr_src);
    cpu.esp -= data_size / 8;

    OPERAND mem;
    mem.type = OPR_MEM;
    mem.sreg = SREG_SS;
    mem.data_size = data_size;
    mem.addr = cpu.esp;
    mem.val = sign_ext(opr_src.val, opr_src.data_size);
    operand_write(&mem);
}

make_instr_impl_1op(push, r, v);
make_instr_impl_1op(push, rm, v);
make_instr_impl_1op(push, i, b);
make_instr_impl_1op(push, i, v);