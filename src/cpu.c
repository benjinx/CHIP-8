#include "cpu.h"

#include <stdlib.h>
#include <stdbool.h>

uint16_t stack[16];
bool keyStates[16];

void push(uint16_t value)
{
    stack[SP] = value;
    ++SP;
}

uint16_t pop()
{
    --SP;
    return stack[SP];
}

// 00E0 - CLS
void CLS(instruction inst)
{

}

// 00EE - RET
void RET(instruction inst)
{
    PC = pop();
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
    // Draw n bytes at position V[x], V[y], from memory starting at I
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
    //RAM[I + 0] = V[inst.x] / 100;
    //RAM[I + 1] = (V[inst.x] % 100) / 10;
    //RAM[I + 2] = V[inst.x] % 10;
}

// Fx55 - LD[I], Vx
void LD_pI_Vx(instruction inst)
{
    //for (int i = 0; i <= inst.x; ++i) {
    //    write_byte(I + i, V[i]);
    //}
}

// Fx65 - LD Vx, [I]
void LD_Vx_pI(instruction inst)
{
    //for (int i = 0; i <= inst.x; ++i) {
    //    V[i] = read_byte(I + i);
    //}
}