#include <stdio.h>
#include "zemu.h"
#include "utils.h"

void print_registers(z80_t *cpu) {
    registers_t *reg = cpu->reg;
    printf("PC  = 0x%04X\n", reg->pc);
    printf("SP  = 0x%04X\n", reg->sp);
    printf("AF  = 0x%04X\tA  = 0x%02X\tF  = 0x%02X\n", reg->af.full, reg->af.msb, reg->af.lsb);
//    printf("AF' = 0x%04X\tA' = 0x%02X\tF' = 0x%02X\n", reg->af1.full, reg->af1.msb, reg->af1.lsb);
    printf("BC  = 0x%04X\tB  = 0x%02X\tC  = 0x%02X\n", reg->bc.full, reg->bc.msb, reg->bc.lsb);
//    printf("BC' = 0x%04X\tB' = 0x%02X\tC' = 0x%02X\n", reg->bc1.full, reg->bc1.msb, reg->bc1.lsb);
    printf("DE  = 0x%04X\tD  = 0x%02X\tE  = 0x%02X\n", reg->de.full, reg->de.msb, reg->de.lsb);
//    printf("DE' = 0x%04X\tD' = 0x%02X\tE' = 0x%02X\n", reg->de1.full, reg->de1.msb, reg->de1.lsb);
    printf("HL  = 0x%04X\tH  = 0x%02X\tL  = 0x%02X\n", reg->hl.full, reg->hl.msb, reg->hl.lsb);
//    printf("HL' = 0x%04X\tH' = 0x%02X\tL' = 0x%02X\n", reg->hl1.full, reg->hl1.msb, reg->hl1.lsb);
//    printf("IX  = 0x%04X\n", reg->ix.full);
//    printf("IY  = 0x%04X\n", reg->iy.full);
    printf("Flags: ");
    print_flags(cpu);
    printf("\n");
    return;
}

void print_flags(z80_t *cpu) {
    uint8_t *flags = cpu->flags;
    printf("%c%c%c%c%c%c%c%c",
           test_bit(flags, FLAG_MASK_SIGN)       ? 'S' : '-',
           test_bit(flags, FLAG_MASK_ZERO)       ? 'Z' : '-',
           test_bit(flags, FLAG_MASK_5)          ? '5' : '-',
           test_bit(flags, FLAG_MASK_HCARRY)     ? 'H' : '-',
           test_bit(flags, FLAG_MASK_3)          ? '3' : '-',
           test_bit(flags, FLAG_MASK_PV)         ? 'P' : '-',
           test_bit(flags, FLAG_MASK_SUBTRACT)   ? 'N' : '-',
           test_bit(flags, FLAG_MASK_CARRY)      ? 'C' : '-');
    return;
}

void load_rom(z80_t *cpu, FILE *romfile) {
    unsigned int count;
    count = fread(cpu->rom, 1, Z80_ROM_SIZE, romfile);
    printf("Loaded %u byte ROM file\n", count);
    return;
}

void dump_rom(z80_t *cpu, uint16_t offset, uint16_t size) {
    if (size > Z80_ROM_SIZE)
        size = Z80_ROM_SIZE;
    
    for (uint8_t *p = cpu->rom + offset; p <= cpu->rom + (size ? size : Z80_ROM_SIZE); p++)
        printf("0x%04X: %02x\n", p - cpu->rom, *p);
    return;
}

void dump_mem(z80_t *cpu, uint16_t offset, uint16_t size) {
    if (size > Z80_MEM_SIZE)
        size = Z80_MEM_SIZE;
    
    for (uint8_t *p = cpu->mem + offset; p <= cpu->mem + (size ? size : Z80_MEM_SIZE); p++)
        printf("0x%04X: %02x\n", p - cpu->mem, *p);
    return;
}

