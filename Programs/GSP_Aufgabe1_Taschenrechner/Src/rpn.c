#include "rpn.h"
#include "display.h"
#include "stack.h"
#include <stdio.h>

static void print_number(int value) {
   char buf[16];
   snprintf(buf, sizeof(buf), "%d", value);
   print_stdout(buf);
}

ErrorCode rpn_number(int value) { return stack_push(value); }

ErrorCode prn_print_top(void) {
   int value = 0;
   ErrorCode err = stack_peek(0, &value);
   if (err != ERR_OK) {
      return err;
   }
   print_number(value);
   return ERR_OK;
}

ErrorCode rpn_duplicate(void) {
   int value = 0;
   ErrorCode err = stack_peek(0, &value);
   if (err != ERR_OK) {
      return err;
   }
   return stack_push(value);
}

void rpn_clear(void) {
   stack_clear();
   clear_stdout();
}