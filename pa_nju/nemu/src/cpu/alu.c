#include "cpu/cpu.h"
#include <stdint.h>

static inline void set_zf_sf_pf(uint32_t res, size_t data_size) {
    cpu.eflags.ZF = res == 0;
    cpu.eflags.SF = (res >> (data_size - 1)) & 1;
    cpu.eflags.PF = !__builtin_parity(res & 0xff);
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_add(src, dest, data_size);
#else
    uint64_t mask = (1ULL << data_size) - 1;
    uint64_t sign = 1ULL << (data_size - 1);

    uint64_t a = dest & mask;
    uint64_t b = src & mask;

    uint64_t sum = a + b;
    uint32_t res = sum & mask;

    cpu.eflags.CF = (sum >> data_size) & 1;
    cpu.eflags.OF = !!(~(a ^ b) & (a ^ res) & sign);
    set_zf_sf_pf(res, data_size);

    return res;
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_adc(src, dest, data_size);
#else
    uint64_t mask = (1ULL << data_size) - 1;
    uint64_t sign = 1ULL << (data_size - 1);

    uint64_t a = dest & mask;
    uint64_t b = src & mask;
    uint64_t cf = cpu.eflags.CF;

    uint64_t sum = a + b + cf;
    uint32_t res = sum & mask;

    cpu.eflags.CF = (sum >> data_size) & 1;
    cpu.eflags.OF = !!(~(a ^ b) & (a ^ res) & sign);
    set_zf_sf_pf(res, data_size);

    return res;
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_sub(src, dest, data_size);
#else
    uint64_t mask = (1ULL << data_size) - 1;
    uint64_t sign = 1ULL << (data_size - 1);

    uint64_t a = dest & mask;
    uint64_t b = src & mask;

    uint32_t res = (a - b) & mask;

    cpu.eflags.CF = a < b;
    cpu.eflags.OF = !!((a ^ b) & (a ^ res) & sign);
    set_zf_sf_pf(res, data_size);

    return res;
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_sbb(src, dest, data_size);
#else
    uint64_t mask = (1ULL << data_size) - 1;
    uint64_t sign = 1ULL << (data_size - 1);

    uint64_t a = dest & mask;
    uint64_t b = src & mask;
    uint64_t cf = cpu.eflags.CF;

    uint32_t res = (a - b - cf) & mask;

    cpu.eflags.CF = a < b + cf;
    cpu.eflags.OF = !!((a ^ b) & (a ^ res) & sign);
    set_zf_sf_pf(res, data_size);

    return res;
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_mul(src, dest, data_size);
#else
    uint64_t mask = (1ULL << data_size) - 1;
    uint64_t res = (dest & mask) * (src & mask);

    cpu.eflags.CF = cpu.eflags.OF = (res >> data_size) != 0;

    return res;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_imul(src, dest, data_size);
#else
    int64_t a = sign_ext_64((uint32_t)dest, data_size);
    int64_t b = sign_ext_64((uint32_t)src, data_size);
    int64_t res = a * b;

    cpu.eflags.CF = cpu.eflags.OF =
        res != sign_ext_64((uint32_t)res, data_size);

    return res;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_div(src, dest, data_size);
#else
    (void)data_size;
    assert(src != 0);
    return dest / src;
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_idiv(src, dest, data_size);
#else
    (void)data_size;
    assert(src != 0);
    return dest / src;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
#ifdef NEMU_REF_ALU
    return __ref_alu_mod(src, dest);
#else
    assert(src != 0);
    return dest % src;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest) {
#ifdef NEMU_REF_ALU
    return __ref_alu_imod(src, dest);
#else
    assert(src != 0);
    return dest % src;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_and(src, dest, data_size);
#else
    uint32_t mask = (1ULL << data_size) - 1;
    uint32_t res = (dest & src) & mask;

    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    set_zf_sf_pf(res, data_size);

    return res;
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_xor(src, dest, data_size);
#else
    uint32_t mask = (1ULL << data_size) - 1;
    uint32_t res = (dest ^ src) & mask;

    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    set_zf_sf_pf(res, data_size);

    return res;
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_or(src, dest, data_size);
#else
    uint32_t mask = (1ULL << data_size) - 1;
    uint32_t res = (dest | src) & mask;

    cpu.eflags.CF = 0;
    cpu.eflags.OF = 0;
    set_zf_sf_pf(res, data_size);

    return res;
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_shl(src, dest, data_size);
#else
    uint64_t mask = (1ULL << data_size) - 1;
    uint64_t a = dest & mask;
    uint32_t count = src & 0x1f;
    uint32_t res = a;

    if (count != 0) {
        uint64_t wide = a << count;
        res = wide & mask;
        cpu.eflags.CF = (wide >> data_size) & 1;
        set_zf_sf_pf(res, data_size);
    }

    return res;
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_shr(src, dest, data_size);
#else
    uint64_t mask = (1ULL << data_size) - 1;
    uint64_t a = dest & mask;
    uint32_t count = src & 0x1f;
    uint32_t res = a;

    if (count != 0) {
        cpu.eflags.CF = (a >> (count - 1)) & 1;
        res = (a >> count) & mask;
        set_zf_sf_pf(res, data_size);
    }

    return res;
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_sar(src, dest, data_size);
#else
    uint64_t mask = (1ULL << data_size) - 1;
    int64_t a = sign_ext_64(dest, data_size);
    uint32_t count = src & 0x1f;
    uint32_t res = dest & mask;

    if (count != 0) {
        cpu.eflags.CF = (a >> (count - 1)) & 1;
        res = ((uint64_t)(a >> count)) & mask;
        set_zf_sf_pf(res, data_size);
    }

    return res;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
    return __ref_alu_sal(src, dest, data_size);
#else
    return alu_shl(src, dest, data_size);
#endif
}
