#include "cpu/instr.h"

make_instr_func(call_near) {
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;
    operand_read(&rel);

    int len = 1 + data_size / 8;
    cpu.esp -= data_size / 8;

    OPERAND mem;
    mem.type = OPR_MEM;
    mem.sreg = SREG_SS;
    mem.data_size = data_size;
    mem.addr = cpu.esp;
    mem.val = eip + len;
    operand_write(&mem);

    print_asm_1("call", "", len, &rel);
    cpu.eip += sign_ext(rel.val, data_size);
    return len;
}

make_instr_func(call_near_indirect) {
    OPERAND rm;
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    operand_read(&rm);

    cpu.esp -= data_size / 8;

    OPERAND mem;
    mem.type = OPR_MEM;
    mem.sreg = SREG_SS;
    mem.data_size = data_size;
    mem.addr = cpu.esp;
    mem.val = eip + len;
    operand_write(&mem);

    print_asm_1("call", "", len, &rm);
    cpu.eip = rm.val;
    return 0;
}
