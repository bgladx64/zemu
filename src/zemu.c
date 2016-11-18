#include <stdio.h>
#include <stdlib.h>
#include "zemu.h"

int main(int argc, char **argv) {
    z80_t *cpu;

    printf("ZEMU: Z80 Emulator\n");

    cpu = setup();
    loop(cpu);
    shutdown(cpu);

    return 0;
}

void loop(z80_t *cpu) {
    for (;;) {
        // get instruction
        // execute instruction
        break;
    }

    return;
}

z80_t *setup() {
    z80_t *cpu = calloc(sizeof(z80_t), 1);
    cpu->reg = calloc(sizeof(registers_t), 1);
    cpu->mem = calloc(Z80_RAM_SIZE, 1);
    cpu->reg->flags = &cpu->reg->af.msb;
    cpu->reg->acc = &cpu->reg->af.lsb;

    cpu->reg->bc.full = 0xBCBC;
    cpu->reg->bc1.full = 0xB1B1;
    cpu->reg->de.full = 0xDEDE;
    cpu->reg->de1.full = 0xD1D1;
    cpu->reg->hl.full = 0x4444;
    cpu->reg->hl1.full = 0x5555;
    cpu->reg->wz.full = 0x6666;
    cpu->reg->wz1.full = 0x7777;
    cpu->reg->af.full = 0xAAAA;
    cpu->reg->af1.full = 0xFFFF;
    *cpu->reg->flags = 0x22;
    *cpu->reg->acc = 0x33;

    print_registers(cpu);

    return cpu;
}

void shutdown(z80_t *cpu) {
    free(cpu->mem);
    free(cpu->reg);
    free(cpu);
    return;
}

void print_registers(z80_t *cpu) {
    registers_t *reg = cpu->reg;
    printf("PC  = 0x%04X\n", reg->pc);
    printf("SP  = 0x%04X\n", reg->sp);
    printf("IX  = 0x%04X\n", reg->ix);
    printf("IY  = 0x%04X\n", reg->iy);
    printf("BC  = 0x%04X\tB  = 0x%02X\tC  = 0x%02X\n", reg->bc.full, reg->bc.lsb, reg->bc.msb);
    printf("BC' = 0x%04X\tB' = 0x%02X\tC' = 0x%02X\n", reg->bc1.full, reg->bc1.lsb, reg->bc1.msb);
    printf("DE  = 0x%04X\tD  = 0x%02X\tE  = 0x%02X\n", reg->de.full, reg->de.lsb, reg->de.msb);
    printf("DE' = 0x%04X\tD' = 0x%02X\tE' = 0x%02X\n", reg->de1.full, reg->de1.lsb, reg->de1.msb);
    printf("HL  = 0x%04X\tH  = 0x%02X\tL  = 0x%02X\n", reg->hl.full, reg->hl.lsb, reg->hl.msb);
    printf("HL' = 0x%04X\tH' = 0x%02X\tL' = 0x%02X\n", reg->hl1.full, reg->hl1.lsb, reg->hl1.msb);
    printf("WZ  = 0x%04X\tW  = 0x%02X\tZ  = 0x%02X\n", reg->wz.full, reg->wz.lsb, reg->wz.msb);
    printf("WZ' = 0x%04X\tW' = 0x%02X\tZ' = 0x%02X\n", reg->wz1.full, reg->wz1.lsb, reg->wz1.msb);
    printf("AF  = 0x%04X\tA  = 0x%02X\tF  = 0x%02X\n", reg->af.full, reg->af.lsb, reg->af.msb);
    printf("AF' = 0x%04X\tA' = 0x%02X\tF' = 0x%02X\n", reg->af1.full, reg->af1.lsb, reg->af1.msb);
    printf("F   = 0x%02X\n", *reg->flags);
    printf("A   = 0x%02X\n", *reg->acc);
    return;
}

