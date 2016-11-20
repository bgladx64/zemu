#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "zemu.h"

int main(int argc, char **argv) {
    z80_t *cpu;
    FILE *romfile;

    if (argc < 2) {
        printf("No ROM file specified\n");
        return -1;
    }

    romfile = fopen(argv[1], "rb");
    if (!romfile) {
        printf("Couldn't open %s\n", argv[1]);
        return -2;
    }

    cpu = setup();
    load_rom(cpu, romfile);
    dump_rom(cpu, 0, 16);
    loop(cpu);
    shutdown(cpu);
    fclose(romfile);

    return 0;
}

void load_rom(z80_t *cpu, FILE *romfile) {
    unsigned int count;
    count = fread(cpu->rom, 1, Z80_ROM_SIZE, romfile);
    printf("Loaded %u byte ROM file\n", count);
    return;
}

void dump_rom(z80_t *cpu, uint16_t offset, uint16_t size) {
    if (size > Z80_ROM_SIZE) {
        size = Z80_ROM_SIZE;
    }
    for (uint8_t *p = cpu->rom + offset; p <= cpu->rom + (size ? size : Z80_ROM_SIZE); p++) {
        printf("0x%04X: %02x\n", p - cpu->rom, *p);
    }
    printf("\n");
}

z80_t *setup() {
    z80_t *cpu = calloc(sizeof(z80_t), 1);
    cpu->reg = calloc(sizeof(registers_t), 1);
    cpu->rom = calloc(Z80_ROM_SIZE, 1);
    cpu->mem = calloc(Z80_MEM_SIZE, 1);
    printf("Allocated CPU at\t%p\t(%lu B)\n", cpu, sizeof(*cpu));
    printf("Allocated registers at\t%p\t(%lu B)\n", cpu->reg, sizeof(*cpu->reg));
    printf("Allocated ROM at\t%p\t(%u B)\n", cpu->rom, Z80_ROM_SIZE);
    printf("Allocated MEM at\t%p\t(%u B)\n", cpu->mem, Z80_MEM_SIZE);

    // Default values;
    cpu->reg->sp = 0xFFFF;

    return cpu;
}

void shutdown(z80_t *cpu) {
    free(cpu->mem);
    free(cpu->rom);
    free(cpu->reg);
    free(cpu);
    return;
}

void print_registers(z80_t *cpu) {
    registers_t *reg = cpu->reg;
    printf("PC  = 0x%04X\n", reg->pc);
    printf("SP  = 0x%04X\n", reg->sp);
    printf("IX  = 0x%04X\n", reg->ix.full);
    printf("IY  = 0x%04X\n", reg->iy.full);
    printf("BC  = 0x%04X\tB  = 0x%02X\tC  = 0x%02X\n", reg->bc.full, reg->bc.msb, reg->bc.lsb);
    printf("BC' = 0x%04X\tB' = 0x%02X\tC' = 0x%02X\n", reg->bc1.full, reg->bc1.msb, reg->bc1.lsb);
    printf("DE  = 0x%04X\tD  = 0x%02X\tE  = 0x%02X\n", reg->de.full, reg->de.msb, reg->de.lsb);
    printf("DE' = 0x%04X\tD' = 0x%02X\tE' = 0x%02X\n", reg->de1.full, reg->de1.msb, reg->de1.lsb);
    printf("HL  = 0x%04X\tH  = 0x%02X\tL  = 0x%02X\n", reg->hl.full, reg->hl.msb, reg->hl.lsb);
    printf("HL' = 0x%04X\tH' = 0x%02X\tL' = 0x%02X\n", reg->hl1.full, reg->hl1.msb, reg->hl1.lsb);
    printf("AF  = 0x%04X\tA  = 0x%02X\tF  = 0x%02X\n", reg->af.full, reg->af.msb, reg->af.lsb);
    printf("AF' = 0x%04X\tA' = 0x%02X\tF' = 0x%02X\n", reg->af1.full, reg->af1.msb, reg->af1.lsb);
    return;
}

