#include "cpu.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mmu.h"
#include "video.h"
#include <stdio.h>

typedef void(*instruction_func_t)(instruction_t);

bool keyStates[16];

// 00E0 - CLS
void CLS(instruction inst)
{
    memset(VRAM, 0, sizeof(VRAM));
}

// 00EE - RET
void RET(instruction inst)
{
    PC = pop();
}

void opcode_0(instruction inst)
{
    if (inst.kk == 0xE0)
    {
        CLS(inst);
    }
    else if (inst.kk == 0xEE)
    {
        RET(inst);
    }
}

// 1nnn - JP addr
void JP_addr(instruction inst)
{
    PC = inst.addr;
}

// 2nnn - CALL addr
void CALL_addr(instruction inst)
{
    push(PC);
    PC = inst.addr;
}

// 3xkk - SE Vx, byte
void SE_Vx_kk(instruction inst)
{
    if (V[inst.x] == inst.kk)
    {
        PC += 2;
    }
}

// 4xkk - SNE Vx, byte
void SNE_Vx_kk(instruction inst)
{
    if (V[inst.x] != inst.kk)
    {
        PC += 2;
    }
}

// 5xy0 - SE Vx, Vy
void SE_Vx_Vy(instruction inst)
{
    if (V[inst.x] == V[inst.y])
    {
        PC += 2;
    }
}

// 6xkk - LD Vx, byte
void LD_Vx_kk(instruction inst)
{
    V[inst.x] = inst.kk;
}

// 7xkk - ADD Vx, byte
void ADD_Vx_kk(instruction inst)
{
    int result = V[inst.x] + inst.kk;
    
    V[0xF] = (result > 0xFF ? 1 : 0);

    V[inst.x] = result;
}

// 8xy0 - LD Vx, Vy
void LD_Vx_Vy(instruction inst)
{
    V[inst.x] = V[inst.y];
}

// 8xy1 - OR Vx, Vy
void OR_Vx_Vy(instruction inst)
{
    V[inst.x] |= V[inst.y];
}

// 8xy2 - AND Vx, Vy
void AND_Vx_Vy(instruction inst)
{
    V[inst.x] &= V[inst.y];
}

// 8xy3 - XOR Vx, Vy
void XOR_Vx_Vy(instruction inst)
{
    V[inst.x] ^= V[inst.y];
}

// 8xy4 - ADD Vx, Vy
void ADD_Vx_Vy(instruction inst)
{
    int result = V[inst.x] + V[inst.y];

    V[0xF] = (result > 0xFF ? 1 : 0);

    V[inst.x] = result;
}

// 8xy5 - SUB Vx, Vy
void SUB_Vx_Vy(instruction inst)
{
    V[0xF] = (V[inst.y] > V[inst.x] ? 1 : 0);
    V[inst.x] -= V[inst.y];
}

// 8xy6 - SHR Vx {, Vy}
void SHR_Vx_Vy(instruction inst)
{
    V[0xF] = V[inst.y] & 0x01;
    V[inst.x] = V[inst.y] >> 1;
}

// 8xy7 - SUBN Vx, Vy
void SUBN_Vx_Vy(instruction inst)
{
    V[0xF] = (V[inst.y] > V[inst.x] ? 0 : 1);
    V[inst.x] -= V[inst.y];
}

// 8xyE - SHL Vx {, Vy}
void SHL_Vx_Vy(instruction inst)
{
    V[0xF] = V[inst.y] & 0x80;
    V[inst.x] = V[inst.y] << 1;
}

void opcode_8(instruction inst)
{
    static instruction_func_t map[16] = {
        [0x0] = LD_Vx_Vy,
        [0x1] = OR_Vx_Vy,
        [0x2] = AND_Vx_Vy,
        [0x3] = XOR_Vx_Vy,
        [0x4] = ADD_Vx_Vy,
        [0x5] = SUB_Vx_Vy,
        [0x6] = SHR_Vx_Vy,
        [0x7] = SUBN_Vx_Vy,
        [0x8] = NULL,
        [0x9] = NULL,
        [0xA] = NULL,
        [0xB] = NULL,
        [0xC] = NULL,
        [0xD] = NULL,
        [0xE] = SHL_Vx_Vy,
        [0xF] = NULL,
    };

    if (map[inst.n]) {
        map[inst.n](inst);
    }
}

// 9xy0 - SNE Vx, Vy
void SNE_Vx_Vy(instruction inst)
{
    if (V[inst.x] != V[inst.y])
    {
        PC += 2;
    }
}

// Annn - LD I, addr
void LD_I_addr(instruction inst)
{
    I = inst.addr;
}

// Bnnn - JP V0, addr
void JP_V0_addr(instruction inst)
{
    PC = inst.addr + V[0];
}

// Cxkk - RND Vx, byte
void RND_Vx_kk(instruction inst)
{
    V[inst.x] = (rand() % 256) & inst.kk;
}

// Dxyn - DRW Vx, Vy, nibble
void DRW_Vx_Vy_n(instruction inst)
{
    // Set flag register to 0
    V[0xF] = 0;

    // Draw n rows of pixels at position V[x], V[y], from memory starting at I
    for (unsigned row = 0; row < inst.n; ++row)
    {
        uint8_t data = read_byte(I + row);

        for (int col = 0; col < 8; ++col)
        {
            int x = (V[inst.x] + col) % WIDTH;
            int y = (V[inst.y] + row) % HEIGHT;

            int pixel = (data & (0x80 >> col));

            if (VRAM[y][x] > 0 && pixel > 0)
            {
                V[0xF] = 1;
            }

            VRAM[y][x] ^= (pixel > 0 ? 0xFF : 0x00);
        }
    }
}

// Ex9E - SKP Vx
void SKP_Vx(instruction inst)
{
    if (keyStates[V[inst.x]])
    {
        PC += 2;
    }
}

// ExA1 - SKNP Vx
void SKNP_Vx(instruction inst)
{
    if (!keyStates[V[inst.x]])
    {
        PC += 2;
    }
}

// Fx07 - LD Vx, DT
void LD_Vx_DT(instruction inst)
{
    V[inst.x] = DT;
}

// Fx0A - LD Vx, K
void LD_Vx_K(instruction inst)
{
    // Wait until key is pressed, once pressed STUFF IT IN X
}

// Fx15 - LD DT, Vx
void LD_DT_Vx(instruction inst)
{
    DT = V[inst.x];
}

// Fx18 - LD ST, Vx
void LD_ST_Vx(instruction inst)
{
    ST = V[inst.x];
}

// Fx1E - ADD I, Vx
void ADD_I_Vx(instruction inst)
{
    I += V[inst.x];
}

// Fx29 - LD F, Vx
void LD_F_Vx(instruction inst)
{
    // F = V[inst.x];
}

// Fx33 - LD B, Vx
void LD_B_Vx(instruction inst)
{
    RAM[I + 0] = V[inst.x] / 100;
    RAM[I + 1] = (V[inst.x] % 100) / 10;
    RAM[I + 2] = V[inst.x] % 10;
}

// Fx55 - LD[I], Vx
void LD_pI_Vx(instruction inst)
{
    for (unsigned i = 0; i <= inst.x; ++i) {
        write_byte(I + i, V[i]);
    }
}

// Fx65 - LD Vx, [I]
void LD_Vx_pI(instruction inst)
{
    for (unsigned i = 0; i <= inst.x; ++i) {
        V[i] = read_byte(I + i);
    }
}

instruction fetch()
{
    instruction inst = {
        .raw = (read_byte(PC) << 8) | read_byte(PC + 1),
    };
    printf("[%03X] OP=%X AAA=%03X X=%X Y=%X KK=%02X\n", PC, inst.opcode, inst.addr, inst.x, inst.y, inst.kk);
    PC += 2;
    return inst;
}

void execute(instruction inst)
{
    static instruction_func_t map[16] =
    {
        [0x0] = opcode_0,
        [0x1] = JP_addr,
        [0x2] = CALL_addr,
        [0x3] = SE_Vx_kk,
        [0x4] = SNE_Vx_kk,
        [0x5] = SE_Vx_Vy,
        [0x6] = LD_Vx_kk,
        [0x7] = ADD_Vx_kk,
        [0x8] = opcode_8,
        [0x9] = SNE_Vx_Vy,
        [0xA] = LD_I_addr,
        [0xB] = JP_V0_addr,
        [0xC] = RND_Vx_kk,
        [0xD] = DRW_Vx_Vy_n,
        //[0xE] = opcode_E,
        //[0xF] = opcode_F,
    };

    if (map[inst.opcode]) {
        map[inst.opcode](inst);
    }
}