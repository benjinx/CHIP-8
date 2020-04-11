#ifndef CPU_H
#define CPU_H

#include "registers.h"

#include <stdint.h>

typedef union
{   
    /// [OP][OP][OP][OP][A][A][A][A][A][A][A][A][A][A][A][A]
    struct {
        uint16_t addr:12; // A 12-bit value, the lowest 12 bits of the instruction
        uint16_t opcode:4; // first 4 bits
    };

    /// [OP][OP][OP][OP][X][X][X][X][Y][Y][Y][Y][N][N][N][N]
    struct {
        uint16_t n:4; // A 4-bit value, the lowest 4 bits of the instruction
        uint16_t y:4; // A 4-bit value, the upper 4 bits of the low byte of the instruction
        uint16_t x:4; // A 4-bit value, the lower 4 bits of the high byte of the instruction
        uint16_t :4;
    };

    /// [OP][OP][OP][OP][X][X][X][X][K][K][K][K][K][K][K][K]
    struct {
        uint16_t kk:8; // An 8 - bit value, the lowest 8 bits of the instruction
        uint16_t :8;
    };
    uint16_t raw;
} instruction;

instruction fetch();
void execute(instruction inst);

#endif // CPU_H