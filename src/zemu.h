#pragma once

#include <stdint.h>

#define Z80_ROM_SIZE 0xFFFF // 65536
#define Z80_MEM_SIZE 0xFFFF // 65536
#define DIE_ON_RST
#define VERBOSE

#define FLAG_BIT_SIGN       7
#define FLAG_BIT_ZERO       6
#define FLAG_BIT_5          5
#define FLAG_BIT_HCARRY     4    // Half carry
#define FLAG_BIT_3          3
#define FLAG_BIT_PV         2    // Parity/overflow
#define FLAG_BIT_SUBTRACT   1
#define FLAG_BIT_CARRY      0

#define FLAG_MASK_SIGN       0x80
#define FLAG_MASK_ZERO       0x40
#define FLAG_MASK_5          0x20
#define FLAG_MASK_HCARRY     0x10    // Half carry
#define FLAG_MASK_3          0x08
#define FLAG_MASK_PV         0x04    // Parity/overflow
#define FLAG_MASK_SUBTRACT   0x02
#define FLAG_MASK_CARRY      0x01


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
    uint8_t *flags;
    uint8_t iff1;
    uint8_t iff2;
    uint8_t im;
    registers_t *reg;
} z80_t;

///////////////////
// Functions
///////////////////
void loop(z80_t *);
void ed_instructions(z80_t *);
z80_t *setup();
void shutdown(z80_t *);
void write_port(z80_t *cpu, uint16_t addr, uint8_t data);

