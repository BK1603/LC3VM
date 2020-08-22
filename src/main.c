#include "lc3.h"
#include "instruction.h"
#include "trap.h"

uint16_t memory[UINT16_MAX];
uint16_t reg[R_COUNT];
int running = 1;

int main(int argc, const char* argv[]) {
  // load arguments
  if (argc < 2) {
    printf("lcr [image-file1] ...\n");
    exit(2);
  }

  for (int j = 1; j < argc; j++) {
    if (!read_image((const char *)argv[j])) {
      printf("Failed to load image: %s\n", argv[j]);
      exit(1);
    }
  }
  // setup
  signal(SIGINT, handle_interrupt);
  disable_input_buffering();

  /* set the PC to starting position */
  /* 0x3000 is the default */
  enum { PC_START = 0x3000 };
  reg[R_PC] = PC_START;

  while (running) {
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t op = instr >> 12;

    switch (op) {
      case OP_ADD:
        add(instr);
        break;
      case OP_AND:
        and(instr);
        break;
      case OP_NOT:
        not(instr);
        break;
      case OP_BR:
        branch(instr);
        break;
      case OP_JMP:
        jump(instr);
        break;
      case OP_JSR:
        jsr(instr);
        break;
      case OP_LD:
        load(instr);
        break;
      case OP_LDI:
        ldi(instr);
        break;
      case OP_LDR:
        ldr(instr);
        break;
      case OP_LEA:
        lea(instr);
        break;
      case OP_ST:
        store(instr);
        break;
      case OP_STI:
        sti(instr);
        break;
      case OP_STR:
        str(instr);
        break;
      case OP_TRAP:
        trap(instr);
        break;
      case OP_RES:
      case OP_RTI:
      default:
        // error out
        abort();
        break;
    }
  }
  // shutdown
  restore_input_buffering();
}
