#include "zemu.h"
#include "utils.h"

static inline void z80_inc(z80_t *cpu, uint8_t *byte) {
    uint8_t *flags = cpu->flags;
    registers_t *reg = cpu->reg;

    (*byte)++;
    // S
    copy_bit(flags, FLAG_MASK_SIGN, (*byte >> 7) & 0x01);
    // Z
    copy_bit(flags, FLAG_MASK_ZERO, *byte ? 0 : 1);
    // H
    copy_bit(flags, FLAG_MASK_HCARRY, *byte & 0x0F ? 0 : 1);
    // V
    copy_bit(flags, FLAG_MASK_PV, *byte == 0x80 ? 1 : 0);
    // N
    clear_bit(flags, FLAG_MASK_SUBTRACT);
    return;
}

static inline void z80_dec(z80_t *cpu, uint8_t *byte) {
    uint8_t *flags = cpu->flags;
    registers_t *reg = cpu->reg;

    (*byte)--;
    // S
    copy_bit(flags, FLAG_MASK_SIGN, (*byte >> 7) & 0x01);
    // Z
    copy_bit(flags, FLAG_MASK_ZERO, *byte ? 0 : 1);
    // H
    copy_bit(flags, FLAG_MASK_HCARRY, (*byte & 0x0F) == 0x0F ? 1 : 0);
    // V
    copy_bit(flags, FLAG_MASK_PV, *byte == 0x7F ? 1 : 0);
    // N
    clear_bit(flags, FLAG_MASK_SUBTRACT);
    return;
}

