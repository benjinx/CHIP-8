#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

extern uint8_t V[16]; // we use these 0-F
extern uint16_t I;
extern uint16_t PC;
extern uint16_t SP;

extern uint8_t DT;
extern uint8_t ST;

#endif // REGISTERS_H