#include "cpu/instr.h"

make_instr_func(leave) {
    cpu.esp = cpu.ebp;

    OPERAND mem;
    mem.type = OPR_MEM;
    mem.sreg = SREG_SS;
    mem.data_size = data_size;
    mem.addr = cpu.esp;
    operand_read(&mem);

    OPERAND ebp;
    ebp.type = OPR_REG;
    ebp.addr = REG_EBP;
    ebp.data_size = data_size;
    ebp.val = mem.val;
    operand_write(&ebp);

    cpu.esp += data_size / 8;
    print_asm_0("leave", "", 1);
    return 1;
}
