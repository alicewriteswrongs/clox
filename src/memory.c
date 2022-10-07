#include "memory.h"

#include <stdlib.h>

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  // if the new size is zero we free the memory
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  // else we can just call realloc to whatever the new size is
  // see https://en.cppreference.com/w/c/memory/realloc
  void* result = realloc(pointer, newSize);

  // `realloc` will return NULL if it's OOM
  // we should handle that
  if (result == NULL) {
    exit(1);
  }

  return result;
}
