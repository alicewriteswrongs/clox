#include "vm.h"

#include <stdio.h>

#include "common.h"
#include "debug.h"

VM vm;

static void resetStack() {
  // set it to point at the first frame
  vm.stackTop = vm.stack;
}

void initVM() { resetStack(); }

void freeVM() {}

void push(Value value) {
  // dereference point, set that spot in memory to that value
  *vm.stackTop = value;
  // increment addres to point at the next spot
  vm.stackTop++;
}

Value pop() {
  // decrement the pointer
  vm.stackTop--;
  // dereference it to get the value there!
  return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
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
        push(constant);
        break;
      }
      case OP_RETURN: {
        printValue(pop());
        printf("\n");
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
