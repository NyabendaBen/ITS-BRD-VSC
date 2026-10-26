#include "stack.h"
#include "errorhandler.h"

static int data[STACK_SIZE];
static int count = 0;

int stack_size(void) { return count; }

void stack_clear(void) { count = 0; }

ErrorCode stack_push(int value) {
   if (count >= STACK_SIZE) {
      return ERR_STACK_OVERFLOW;
   }
   data[count++] = value;
   return ERR_OK;
}

ErrorCode stack_pop(int *value) {
   if (count <= 0) {
      return ERR_STACK_UNDERFLOW;
   }
   *value = data[--count];
   return ERR_OK;
}

ErrorCode stack_peek(int depth, int *value) {
   if (depth < 0 || depth >= count) {
      return ERR_STACK_UNDERFLOW;
   }
   *value = data[count - 1 - depth];
   return ERR_OK;
}