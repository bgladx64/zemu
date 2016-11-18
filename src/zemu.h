#pragma once

#include <stdint.h>

#define Z80_RAM_SIZE 32768

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
    uint16_t ix;
    uint16_t iy;
    register_pair_t bc;
    register_pair_t bc1;
    register_pair_t de;
    register_pair_t de1;
    register_pair_t hl;
    register_pair_t hl1;
    register_pair_t wz;
    register_pair_t wz1;
    register_pair_t af;
    register_pair_t af1;
    uint8_t *flags;
    uint8_t *acc;
} registers_t;

typedef struct z80 {
    void *mem;
    registers_t *reg;
} z80_t;

void loop(z80_t *);
z80_t *setup();
void shutdown(z80_t *);
void print_registers(z80_t *);

