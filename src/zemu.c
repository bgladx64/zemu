#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "zemu.h"
#include "utils.h"

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
    dump_rom(cpu, 0, 40);
    loop(cpu);
    shutdown(cpu);
    fclose(romfile);

    return 0;
}

void write_port(z80_t *cpu, uint16_t addr, uint8_t data) {
    printf("WRITEOUT: (0x%04X) 0x%02X\n", addr, data);
    *(cpu->mem + addr) = data;
    return;
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
    cpu->flags = &cpu->reg->af.lsb;
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

