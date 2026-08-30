#include "cpu/instr.h"

make_instr_func(ret_near) {
    OPERAND mem;
    mem.type = OPR_MEM;
    mem.sreg = SREG_SS;
    mem.data_size = data_size;
    mem.addr = cpu.esp;
    operand_read(&mem);

    cpu.esp += data_size / 8;
    print_asm_0("ret", "", 1);
    cpu.eip = mem.val;
    return 0;
}

make_instr_func(ret_near_imm16) {
    OPERAND mem;
    mem.type = OPR_MEM;
    mem.sreg = SREG_SS;
    mem.data_size = data_size;
    mem.addr = cpu.esp;
    operand_read(&mem);
    cpu.esp += data_size / 8;

    OPERAND imm;
    imm.type = OPR_IMM;
    imm.sreg = SREG_CS;
    imm.data_size = 16;
    imm.addr = eip + 1;
    operand_read(&imm);
    cpu.esp += imm.val;

    print_asm_1("ret", "", 3, &imm);
    cpu.eip = mem.val;
    return 0;
}
