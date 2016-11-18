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
    z80_t *cpu = malloc(sizeof(z80_t));
    cpu->reg = malloc(sizeof(registers_t));
    cpu->mem = malloc(Z80_RAM_SIZE);

    return cpu;
}

void shutdown(z80_t *cpu) {
    free(cpu->mem);
    free(cpu->reg);
    free(cpu);
    return;
}

