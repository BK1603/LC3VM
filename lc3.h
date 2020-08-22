#ifndef LC3_H
#define LC3_H

/* memory 16bit x 65536 128kb */
extern uint16_t memory[UINT16_MAX];

/* registers */
enum {
  R_R0 = 0,
  R_R1,
  R_R2,
  R_R3,
  R_R4,
  R_R5,
  R_R6,
  R_R7,
  R_PC, /* program counter */
  R_COND, /* condition register */
  R_COUNT, /* counter */
};

extern uint16_t reg[R_COUNT];

/* instruction set */
enum {
  OP_BR=0,    /* branch */
  OP_ADD,
  OP_LD,
  OP_ST,
  OP_JSR,     /* jump register */
  OP_AND,     /* bitwise and */
  OP_LDR,     /* load register */
  OP_STR,     /* store register */
  OP_RTI,     /* unused */
  OP_NOT,
  OP_LDI,     /* load indirect */
  OP_STI,     /* store indirect */
  OP_JMP,
  OP_RES,     /* reserved */
  OP_LEA,
  OP_TRAP
};

/* flags */
enum {
  FL_POS = 1 << 0;
  FL_ZRO = 1 << 1;
  FL_NEG = 1 << 2;
};

/* Syscalls */
enum {
  TRAP_GETC = 0x20,
  TRAP_OUT = 0x21,
  TRAP_PUTS = 0x22,
  TRAP_IN = 0x23,
  TRAP_PUTSP = 0x24,
  TRAP_HALT = 0x25
};

#endif
