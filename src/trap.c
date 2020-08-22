#include "lc3.h"
#include "trap.h"

void trap_getc() {
  reg[R_R0] = (uint16_t)getchar();
}

void trap_out() {
  putc((char)reg[R_R0], stdout);
  fflush(stdout);
}

void trap_puts() {
  uint16_t *c = memory + reg[R_R0];
  while (*c) {
    putc((char)*c, stdout);
    c++;
  }
  fflush(stdout);
}

void trap_in() {
  printf("Here's your prompt: ");
  char c = getchar();
  putc(c, stdout);
  reg[R_R0] = (uint16_t)getchar();
}

void trap_putsp() {
  uint16_t *c = memory + reg[R_R0];
  while (*c) {
    char ch = (*c) & 0xFF;
    putc(ch, stdout);
    ch = *c >> 8;
    if (ch) {
      putc(ch, stdout);
    }
    c++;
  }
  fflush(stdout);
}

void trap_halt() {
  puts("HALTED");
  fflush(stdout);
  running = 0;
}

void trap(uint16_t instr) {
  switch (instr & 0xFF) {
    case TRAP_GETC:
      trap_getc();
      break;
    case TRAP_OUT:
      trap_out();
      break;
    case TRAP_PUTS:
      trap_puts();
      break;
    case TRAP_IN:
      trap_in();
      break;
    case TRAP_PUTSP:
      trap_putsp();
      break;
    case TRAP_HALT:
      trap_putsp();
      break;
  }
}
