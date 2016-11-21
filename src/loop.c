#include <stdio.h>
#include <stdint.h>
#include "zemu.h"
#include "utils.h"
#include "ops.h"

// TODO next:
// 
//
// TODO general:
// cycle counter
// interrupts
// flags
// LD: 8/16
// LD from mem

void loop(z80_t *cpu) {
    uint8_t *instr;
    uint8_t *rom = cpu->rom;
    registers_t *reg = cpu->reg;
    uint8_t *flags = cpu->flags;
    uint8_t hc, ov;
    int bail = 0;

    for (;;) {
        // get instruction
#ifdef VERBOSE
        printf("\n0x%04X: ", reg->pc);
#endif
        instr = rom + reg->pc++;
#ifdef VERBOSE
        printf("(0x%02X) ", *instr);
#endif

        // execute instruction
        switch (*instr) {
            case 0x00:
                // NOP
                // --------
#ifdef VERBOSE
                printf("NOP\n");
#endif
                break;
            case 0x01:
                // LD BC, nn
                // --------
#ifdef VERBOSE
                printf("LD BC, %04p\n", *(uint16_t *)(rom + reg->pc));
#endif
                reg->bc.full = *(uint16_t *)(rom + reg->pc);
                reg->pc += 2;
                break;
            case 0x02:
                // LD (BC), A
                // --------
#ifdef VERBOSE
                printf("LD (BC), A\n");
#endif
                *(rom + reg->bc.full) = reg->af.msb;
                break;
            case 0x03:
                // INC BC
                // --------
#ifdef VERBOSE
                printf("INC BC\n");
#endif
                reg->bc.full++;
                break;
            case 0x04:
                // INC B
                // SZ5H3VN-
#ifdef VERBOSE
                printf("INC B\n");
#endif
                z80_inc(cpu, &reg->bc.msb);
                break;
            case 0x05:
                // DEC B
                // SZ5H3VN-
#ifdef VERBOSE
                printf("DEC B\n");
#endif
                z80_dec(cpu, &reg->bc.msb);
                break;
            case 0x06:
                // LD B, n
                // --------
#ifdef VERBOSE
                printf("LD B, 0x%02x\n", *(rom + reg->pc));
#endif
                reg->bc.msb = *(rom + reg->pc++);
                break;
            case 0x09:
                // ADD HL, BC
                // --***-0C
                // TODO: 5, H, 3
#ifdef VERBOSE
                printf("ADD HL, BC\n");
#endif
                uint32_t carry = ((uint32_t)reg->hl.full + (uint32_t)reg->bc.full) & 0x100;
                reg->hl.full += reg->bc.full;
                // N
                clear_bit(flags, FLAG_MASK_SUBTRACT);
                // C
                copy_bit(flags, FLAG_MASK_CARRY, carry ? 1 : 0);
                break;
            case 0x0A: // LD A, (BC)
                reg->af.msb = *(rom + reg->bc.full);
                break;
            case 0x0B: // DEC BC
                reg->bc.full--;
                break;
            case 0x0C:
                // INC C
                // SZ5H3VN-
#ifdef VERBOSE
                printf("INC C\n");
#endif
                z80_inc(cpu, &reg->bc.lsb);
                break;
            case 0x0D:
                // DEC C
                // SZ5H3VN-
#ifdef VERBOSE
                printf("DEC C\n");
#endif
                z80_dec(cpu, &reg->bc.lsb);
                break;
            case 0x0E:
                // LD C, n
#ifdef VERBOSE
                printf("LD C, 0x%02x\n", *(rom + reg->pc));
#endif
                reg->bc.lsb = *(rom + reg->pc++);
                break;
            case 0x16: // LD D, n
                reg->de.msb = *(rom + reg->pc++);
                break;
            case 0x18: // JR a
#ifdef VERBOSE
                printf("JR %d\n", *(int8_t *)(rom + reg->pc)+1);
#endif
                reg->pc += *(int8_t *)(rom + reg->pc)+1;
                break;
            case 0x1E: // LD E, n
                reg->de.lsb = *(rom + reg->pc++);
                break;
            case 0x20:
                // JR NZ, a
#ifdef VERBOSE
                printf("JR NZ, %d\n", *(int8_t *)(rom + reg->pc)+1);
#endif
                if (!test_bit(flags, FLAG_MASK_ZERO)) reg->pc += *(int8_t *)(rom + reg->pc)+1;
                else reg->pc++;
                break;
            case 0x21:
                // LD HL, nn
#ifdef VERBOSE
                printf("LD HL, 0x%04X\n", *(uint16_t *)(rom + reg->pc));
#endif
                reg->hl.full = *(uint16_t *)(rom + reg->pc);
                reg->pc += 2;
                break;
            case 0x28:
                // JR Z, a
#ifdef VERBOSE
                printf("JR Z, %d\n", *(int8_t *)(rom + reg->pc)+1);
#endif
                if (test_bit(flags, FLAG_MASK_ZERO)) reg->pc += *(int8_t *)(rom + reg->pc)+1;
                else reg->pc++;
                break;
            case 0x30:
                // JR NC, a
#ifdef VERBOSE
                printf("JR NC, %d\n", *(int8_t *)(rom + reg->pc)+1);
#endif
                if (!test_bit(flags, FLAG_MASK_CARRY)) reg->pc += (*(int8_t *)(rom + reg->pc)+1);
                else reg->pc++;
                break;
            case 0x31:
                // LD SP, nn
#ifdef VERBOSE
                printf("LD SP, 0x%04x\n", *(uint16_t *)(rom + reg->pc));
#endif
                reg->sp = *(uint16_t *)(rom + reg->pc);
                reg->pc += 2;
                break;
            case 0x32:
                // LD (nn), A
#ifdef VERBOSE
                printf("LD 0x%04X, A\n", *(uint16_t *)(rom + reg->pc));
#endif
                *(rom + *(uint16_t *)(rom + reg->pc)) = reg->af.msb;
                reg->pc += 2;
                break;
            case 0x3A:
                // LD A, (nn)
#ifdef VERBOSE
                printf("LD A, 0x%04X\n", *(uint16_t *)(rom + reg->pc));
#endif
                reg->af.msb = *(rom + *(uint16_t *)(rom + reg->pc));
                reg->pc += 2;
                break;
            case 0x3D:
                // DEC A
#ifdef VERBOSE
                printf("DEC A\n");
#endif
                z80_dec(cpu, &reg->af.msb);
                break;
            case 0x3E:
                // LD A, n
#ifdef VERBOSE
                printf("LD A, 0x%02x\n", *(rom + reg->pc));
#endif
                reg->af.msb = *(rom + reg->pc++);
                break;
            case 0x47:
                // LD B, A
#ifdef VERBOSE
                printf("LD B, A\n");
#endif
                reg->bc.msb = reg->af.msb;
                break;
            case 0x78:
                // LD A, B
#ifdef VERBOSE
                printf("LD A, B\n");
#endif
                reg->af.msb = reg->bc.msb;
                break;
            case 0x80: // ADD A, B
                reg->af.msb += reg->bc.msb;
                break;
            case 0x90:
                // SUB B
                // SZ5H3VNC
#ifdef VERBOSE
                printf("SUB B\n");
#endif
                reg->af.msb -= reg->bc.msb;
                // S
                copy_bit(flags, FLAG_MASK_SIGN, test_bit(&reg->af.msb, FLAG_MASK_SIGN) ? 1 : 0);
                // Z
                if (reg->af.msb == 0) set_bit(flags, FLAG_MASK_ZERO);
                else clear_bit(flags, FLAG_MASK_ZERO);
                // N
                set_bit(flags, FLAG_MASK_SUBTRACT);
                // C
                if ((int8_t)reg->af.msb < 0) set_bit(flags, FLAG_MASK_CARRY);
                else clear_bit(flags, FLAG_MASK_CARRY);
                break;
            case 0xC1: // POP BC
                reg->bc.full = *(uint16_t *)(rom + reg->sp);
                reg->sp += 2;
                break;
            case 0xC3:
                // JP nn
#ifdef VERBOSE
                printf("JP 0x%04x\n", *(uint16_t *)(rom + reg->pc));
#endif
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
            case 0xD3:
                // OUT (n), A
#ifdef VERBOSE
                printf("OUT (0x%02x), A\n", *(rom + reg->pc));
#endif
                write_port(cpu, (reg->af.full & 0xFF00) + *(rom + reg->pc++), reg->af.msb);
                break;
            case 0xED:
                // Extended instructions
                ed_instructions(cpu);
                break;
            case 0xF1: // POP AF
                reg->af.full = *(uint16_t *)(rom + reg->sp);
                reg->sp += 2;
                break;
            case 0xF3:
                // DI
#ifdef VERBOSE
                printf("DI\n");
#endif
                cpu->iff1 = 0;
                cpu->iff2 = 0;
                break;
            case 0xFF: // RST 38h
#ifdef VERBOSE
                printf("RST 38h\n");
#endif
                reg->pc = 0x38;
                break;
            default:
                printf("\nERROR: unrecognized instruction 0x%02X\n\n", *instr);
                bail = 1;
                break;
        }

        print_registers(cpu);

#ifdef DIE_ON_RST
        if (*instr == 0xFF || *instr == 0xEF || *instr == 0xDF || *instr == 0xCF ||
            *instr == 0xF7 || *instr == 0xE7 || *instr == 0xD7 || *instr == 0xC7)
        {
            break;
        }
        if (bail)
            break;
#endif
    }

    return;
}

void ed_instructions(z80_t *cpu) {
    uint8_t *instr;
    uint8_t *rom = cpu->rom;
    registers_t *reg = cpu->reg;
    uint8_t *flags = cpu->flags;
    uint8_t hc, ov;

    instr = rom + reg->pc++;
#ifdef VERBOSE
    printf("(0x%02XED) ", *instr);
#endif

    switch (*instr) {
        case 0x56:
            // IM 1
#ifdef VERBOSE
            printf("IM 1\n");
#endif
            cpu->im = 1;
            break;
        case 0xB3:
            // OTIR
#ifdef VERBOSE
            printf("OTIR\n");
#endif
            while (reg->bc.msb) {
                write_port(cpu, reg->bc.lsb, *(rom + reg->hl.full++));
                reg->bc.msb--;
            }
            
            // Z
            set_bit(flags, FLAG_MASK_ZERO);
            // N
            set_bit(flags, FLAG_MASK_SUBTRACT);
            break;
        default:
            printf("\nERROR: unrecognized instruction 0xED%02X\n\n", *instr);
            break;
    }
    return;
}

