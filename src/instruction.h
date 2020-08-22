#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

void add(uint16_t);
void and(uint16_t);
void branch(uint16_t);
void jump(uint16_t);
void jsr(uint16_t);
void load(uint16_t);
void ldi(uint16_t);
void ldr(uint16_t);
void lea(uint16_t);
void not(uint16_t);
void store(uint16_t);
void sti(uint16_t);
void str(uint16_t);

#endif
