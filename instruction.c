#include "instruction.h"

/* Extend bit_count to 16 bits */
uint16_t sign_extend(uint16_t x, int bit_count) {
  // if x is negative
  if ((x >> (bit_count - 1)) & 1) {
    x |= (0xFFFF << bit_count);
  }
  return x;
}

void update_flags(uint16_t r) {
  if (reg[r] == 0) {
    reg[R_COND] = FL_ZRO;
  } else if (reg[r] >> 15) {
    reg[R_COND] = FL_NEG;
  } else {
    reg[R_COND] = FL_POS;
  }
}

/*
 * Add instruction has the following format:
 * 4 bit opcode | 3 bit dest reg | 3 bit src reg | 0 | 00 | 3 bit src2 |
 * 4 bit opcode | 3 bit dest reg | 3 bit src reg | 1 | 5 bit imm value |
 */
void add(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t imm_flag = (instr >> 5) & 0x1;

  if (imm_flag) {
    uint16_t imm5 = sign_extend(instr & 0x1F, 5);
    reg[r0] = reg[r1] + imm5;
  } else {
    uint16_t r2 = instr & 0x7;
    reg[r0] = reg[r1] + reg[r2];
  }

  update_flags(r0);
}

/*
 * And instruction has the following format:
 * 4 bit opcode | 3 bit dest reg | 3 bit src reg | 0 | 00 | 3 bit src2 |
 * 4 bit opcode | 3 bit dest reg | 3 bit src reg | 1 | 5 bit imm value |
 */
void and(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 0) & 0x7;
  uint16_t imm_flag = (instr >> 5) & 0x1;

  if (imm_flag) {
    uint16_t imm5 = sign_extend(instr & 0x1F, 5);
    reg[r0] = reg[r0] & reg[r1];
  } else {
    uint16_t r2 = instr & 0x7;
    reg[r0] = reg[r0] & reg[r2];
  }

  update_flags(r0);
}

/*
 * Branch instruction has the following format:
 * 4 bit opcode | 3 bit flag | 9 bit offset |
 */
void branch(uint16_t instr) {
  uint16_t flag = (instr >> 9) & 0x7;
  uint16_t PCoffset9 = sign_extend(instr & 0x1FF, 9);
  if (reg[R_COND] == flag) {
    reg[R_PC] += PCoffset9;
  }
}

/*
 * Jump instruction has the following format:
 * 4 bit opcode | 000 | 3 bit base reg | 000000 |
 * if base reg = 111, i.e. reg 7, it becomes the
 * ret instruction.
 */
void jump(uint16_t instr) {
  uint16_t BaseR = (instr >> 6) & 0x7;
  reg[R_PC] = reg[BaseR];
}

/*
 * Jump to Subroutine instruction has the following formats:
 * 4 bit opcode | 1 | 11 bit offset |
 * 4 bit opcode | 0 | 00 | 3 bit base reg | 000000 |
 */
void jsr(uint16_t instr) {
  uint16_t flag = (instr >> 11) & 0x1;
  reg[R_PC]++;
  reg[R_R7] = reg[R_PC];
  if (flag) {
    uint16_t PCoffset11 = sign_extend(instr & 0x3FF, 11);
    reg[R_PC] = reg[R_PC] + PCoffset11;
  } else {
    uint16_t BaseR = (instr >> 6) & 0x7;
    reg[R_PC] = reg[BaseR];
  }
}

/*
 * Load instruction has the following format:
 * 4 bit opcode | 3 bit dest reg | 9 bit offset |
 */
void ld(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t PCoffset9 = sign_extend(instr & 0x1FF, 9);
  reg[r0] = memory[reg[R_PC] + PCoffset];

  update_flags(r0);
}

/*
 * Load indirect instruction has the following format:
 * 4 bit opcode | 3 bit dest reg | 9 bit offset |
 */
void ldi(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t PCoffset9 = sign_extend(instr & 0x1FF, 9);
  reg[r0] = memory[memory[reg[R_PC] + PCoffset]];

  update_flags(r0);
}

/*
 * Load base+offset instruction has the following format:
 * 4 bit opcode | 3 bit dest reg | 3 bit base reg | 6 bit offset |
 */
void ldr(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t BaseR = (instr >> 6) & 0x2F;
  uint16_t Offset6 = sign_extend(instr & 0x2F, 6);
  reg[r0] = memory[reg[BaseR] + Offset6];

  update_flags(r0);
}

/*
 * Load effective afress has the following format:
 * 4 bit opcode | 3 bit dest reg | 9 bit offset |
 */
void lea(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t PCoffset9 = sign_extend(instr & 0x1FF, 9);
  reg[r0] = reg[R_PC] + PCoffset9;

  update_flags(r0);
}

/*
 * Not instruction has the following format:
 * 4 bit opcode | 3 bit dest reg | 3 bit src reg | 111111 |
 */
void not(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  reg[r0] = ~reg[r1];

  update_flags(r0);
}

/*
 * Store instruction has the following format:
 * 4 bit opcode | 3 bit src reg | 9 bit offset |
 */
void store(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t PCoffset9 = sign_extend(instr & 0x1FF, 9);
  memory[PC + PCoffset9] = reg[r0];
}

/*
 * Store indirect has the following format:
 * 4 bit opcode | 3 bit source reg | 9 bit offset |
 */
void sti(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t PCoffset9 = sign_extend(instruction & 0x1FF, 9);
  memory[memory[PC + PCoffset9]] = reg[r0];
}

/*
 * Store base + offset instruction has the following format:
 * 4 bit opcode | 3 bit src reg | 9 bit Offset6 |
 */
void str(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t PCoffset9 = sign_extend(instr & 0x2F, 6);
  memory[reg[r1] + PCoffset9] = reg[r0];
}
