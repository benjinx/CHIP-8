#ifndef CPU_H
#define CPU_H

#include "registers.h"

#include <stdint.h>

typedef struct
{   
    union {
        /// [OP][OP][OP][OP][A][A][A][A][A][A][A][A][A][A][A][A]
        struct {
            unsigned addr:12; // A 12-bit value, the lowest 12 bits of the instruction
            unsigned opcode:4; // first 4 bits
        };

        /// [OP][OP][OP][OP][X][X][X][X][Y][Y][Y][Y][N][N][N][N]
        struct {
            unsigned n:4; // A 4-bit value, the lowest 4 bits of the instruction
            unsigned y:4; // A 4-bit value, the upper 4 bits of the low byte of the instruction
            unsigned x:4; // A 4-bit value, the lower 4 bits of the high byte of the instruction
            unsigned :4;
        };

        /// [OP][OP][OP][OP][X][X][X][X][K][K][K][K][K][K][K][K]
        struct {
            unsigned kk:8; // An 8 - bit value, the lowest 8 bits of the instruction
            unsigned :8;
        };
        uint16_t raw;
    };
} instruction;

#endif // CPU_H