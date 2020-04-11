#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include <stdbool.h>

extern uint8_t RAM[0x1000];
extern uint16_t Stack[16];

void push(uint16_t value);
uint16_t pop();

uint8_t read_byte(uint16_t addr);
void write_byte(uint16_t addr, uint8_t data);

bool LoadROM(const char* filename);

#endif // MMU_H