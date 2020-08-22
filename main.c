#include "lc3.h"
#include "instruction.h"

uint16_t memory[UINT16_MAX];
uint16_t reg[R_COUNT];

int main(int argc, const char* argv[]) {
  // load arguments
  if (argc < 2) {
    printf("lcr [image-file1] ...\n");
    exit(2);
  }

  for (int j = 1; j < argc; j++) {
    if (!read_image(argv[j])) {
      printf("Failed to load image: %s\n", argv[j]);
      exit(1);
    }
  }
  // setup

  /* set the PC to starting position */
  /* 0x3000 is the default */
  enum { PC_START = 0x3000 };
  reg[R_OC] = PC_START;

  int running = 1;
  while (running ) {
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t op = instr >> 12;

    switch (op) {
      case OP_ADD:
        add(instr);
        break;
      case OP_AND;
        and(instr);
        break;
      case OP_NOT:
        not(instr);
        break;
      case OP_BR:
        branch(instr);
        break;
      case OP_JMP:
        // jmp
        break;
      case OP_JSR:
        // jsr
        break;
      case OP_LD:
        // ld
        break;
      case OP_LDI:
        // ldi
        break;
      case OP_LDR:
        // ldr
        break;
      case OP_LEA:
        // lea
        break;
      case OP_ST:
        // store
        break;
      case OP_STI:
        // store indirect
        break;
      case OP_STR:
        // st reg
        break;
      case OP_TRAP:
        // trap
        break;
      case OP_RES:
      case OP_RTI:
      default:
        // error out
        break;
    }
  }
  // shutdown
}
