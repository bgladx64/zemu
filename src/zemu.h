#pragma once

#include <stdint.h>

#define Z80_ROM_SIZE 0xFFFF // 65536
#define Z80_MEM_SIZE 0xFFFF // 65536

///////////////////
// Data Structures
///////////////////
typedef union register_pair {
    uint16_t full;
    struct {
        uint8_t lsb;
        uint8_t msb;
    };
} register_pair_t;

typedef struct registers {
    uint16_t pc;
    uint16_t sp;
    register_pair_t ix;
    register_pair_t iy;
    register_pair_t af;
    register_pair_t af1;
    register_pair_t bc;
    register_pair_t bc1;
    register_pair_t de;
    register_pair_t de1;
    register_pair_t hl;
    register_pair_t hl1;
    uint8_t i;
    uint8_t r;
} registers_t;

typedef struct z80 {
    uint8_t *rom;
    uint8_t *mem;
    registers_t *reg;
} z80_t;

///////////////////
// Functions
///////////////////
void loop(z80_t *);
z80_t *setup();
void shutdown(z80_t *);
void print_registers(z80_t *);
void load_rom(z80_t *, FILE *);
void dump_rom(z80_t *, uint16_t, uint16_t);

