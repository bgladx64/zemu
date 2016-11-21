#pragma once

#include "zemu.h"

void print_registers(z80_t *cpu);
void print_flags(z80_t *cpu);
void load_rom(z80_t *cpu, FILE *romfile);
void dump_rom(z80_t *cpu, uint16_t offset, uint16_t size);
void dump_mem(z80_t *cpu, uint16_t offset, uint16_t size);

// Bit operations
static inline uint8_t test_bit(uint8_t *byte, uint8_t mask) {
    return *byte & mask;
}

static inline void set_bit(uint8_t *byte, uint8_t mask) {
    *byte |= mask;
}

static inline void clear_bit(uint8_t *byte, uint8_t mask) {
    *byte &= ~mask; 
}

static inline void toggle_bit(uint8_t *byte, uint8_t mask) {
    *byte ^= mask;
}

static inline void copy_bit(uint8_t *byte, uint8_t mask, int8_t value) {
    *byte ^= (-value ^ *byte) & mask;
}

