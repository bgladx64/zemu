#include <stdio.h>
#include <stdint.h>
#include "zemu.h"

// TODO:
// cycle counter
// interrupts
// flags
// LD: 8/16
// LD from mem
// inc
// dec

void loop(z80_t *cpu) {
    uint8_t *instr;
    uint8_t *rom = cpu->rom;
    registers_t *reg = cpu->reg;

    for (;;) {
        // get instruction
        instr = rom + reg->pc++;
        printf("Read instruction 0x%02x\n", *instr);

        // execute instruction
        switch (*instr) {
            case 0x00: // NOP
                break;
            case 0x01: // LD BC, nn
                reg->bc.full = *(uint16_t *)(rom + reg->pc);
                reg->pc += 2;
                break;
            case 0x02: // LD (BC), A
                *(rom + reg->bc.full) = reg->af.msb;
                break;
            case 0x03: // INC BC
                reg->bc.full++;
                break;
            case 0x04: // INC B
                reg->bc.msb++;
                break;
            case 0x05: // DEC B
                reg->bc.msb--;
                break;
            case 0x06: // LD B, n
                reg->bc.msb = *(rom + reg->pc++);
                break;
            case 0x09: // ADD HL, BC
                reg->hl.full += reg->bc.full;
                break;
            case 0x0A: // LD A, (BC)
                reg->af.msb = *(rom + reg->bc.full);
                break;
            case 0x0B: // DEC BC
                reg->bc.full--;
                break;
            case 0x0C: // INC C
                reg->bc.lsb++;
                break;
            case 0x0D: // DEC C
                reg->bc.lsb--;
                break;
            case 0x0E: // LD C, n
                reg->bc.lsb = *(rom + reg->pc++);
                break;
            case 0x16: // LD D, n
                reg->de.msb = *(rom + reg->pc++);
                break;
            case 0x1E: // LD E, n
                reg->de.lsb = *(rom + reg->pc++);
                break;
            case 0x31: // LD SP, nn
                reg->sp = *(uint16_t *)(rom + reg->pc);
                reg->pc += 2;
                break;
            case 0x3E: // LD A, n
                reg->af.msb = *(rom + reg->pc++);
                break;
            case 0x80: // ADD A, B
                reg->af.msb += reg->bc.msb;
                break;
            case 0xC1: // POP BC
                reg->bc.full = *(uint16_t *)(rom + reg->sp);
                reg->sp += 2;
                break;
            case 0xC3: // JP nn
                reg->pc = *(uint16_t *)(rom + reg->pc);
                break;
            case 0xC5: // PUSH BC
                reg->sp -= 2;
                *(uint16_t *)(rom + reg->sp) = reg->bc.full;
                break;
            case 0xD1: // POP DE
                reg->de.full = *(uint16_t *)(rom + reg->sp);
                reg->sp += 2;
                break;
            case 0xFF:
                break;
            default:
                printf("Error: unrecognized instruction 0x%02x\n", *instr);
                break;
        }

        print_registers(cpu);

        if (*instr == 0xFF) {
            break;
        }
    }

    return;
}

