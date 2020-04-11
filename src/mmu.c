#include "mmu.h"
#include "registers.h"
#include <stdio.h>

uint8_t RAM[0x1000];
uint16_t Stack[16];

void push(uint16_t value)
{
    Stack[SP] = value;
    ++SP;
}

uint16_t pop()
{
    --SP;
    return Stack[SP];
}

uint8_t read_byte(uint16_t addr)
{
    return RAM[addr];
}

void write_byte(uint16_t addr, uint8_t data)
{
    RAM[addr] = data;
}

bool LoadROM(const char* filename)
{
    FILE* pFile;

    pFile = fopen(filename, "rb");

    if (!pFile)
    {
        fprintf(stderr, "YOU DID SOEMTHING REALLY BAD\n");
        return false;
    }

    fseek(pFile, 0, SEEK_END);

    size_t size = ftell(pFile);

    fseek(pFile, 0, SEEK_SET);

    // 0x0000-0x01FF (0x000-0x1FF) is reserved for system

    size_t bytesRead = fread(&RAM[0x200], 1, size, pFile);

    fclose(pFile);

    return true;
}