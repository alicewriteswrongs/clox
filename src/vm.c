#include "vm.h"

#include <stdio.h>

#include "common.h"
#include "debug.h"

VM vm;

void initVM() {}

void freeVM() {}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    // disassemble the current instruction
    //
    // some pointer pointer math here - `vm.chunk->code`
    // points at the start of the array of opcodes. The
    // difference between that and the current value of
    // `vm.ip` gives the right offset for the current byte
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        printValue(constant);
        printf("\n");
        break;
      }
      case OP_RETURN: {
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(Chunk* chunk) {
  vm.chunk = chunk;
  vm.ip    = vm.chunk->code;
  return run();
}
