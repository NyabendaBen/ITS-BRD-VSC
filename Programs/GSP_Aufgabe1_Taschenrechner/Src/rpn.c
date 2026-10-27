#include "rpn.h"
#include "display.h"
#include "stack.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

static void print_number(int value) {
   char buf[PRINT_SIZE];
   snprintf(buf, sizeof(buf), "%d", value);
   print_stdout(buf);
}

ErrorCode rpn_number(int value) { return stack_push(value); }

ErrorCode rpn_print_top(void) {
   int value = 0;
   ErrorCode err = stack_peek(0, &value);
   if (err != ERR_OK) {
      return err;
   }
   print_number(value);
   return ERR_OK;
}

ErrorCode rpn_print_three(void) {
   int current_size = stack_size();
   int limit = 3;
   if (current_size == 0) {
      return ERR_STACK_UNDERFLOW;
   }
   if (current_size < limit) {
      limit = current_size;
   }
   for (int depth = 0; depth < limit; depth++) {
      int value = 0;
      (void)stack_peek(depth, &value);
      print_number(value);
   }
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

ErrorCode rpn_swap(void) {
   int top = 0;
   int bottom = 0;
   ErrorCode err = stack_pop(&top);
   if (err != ERR_OK) {
      return err;
   }
   err = stack_pop(&bottom);
   if (err != ERR_OK) {
      return err;
   }
   (void)stack_push(top);
   (void)stack_push(bottom);
   return ERR_OK;
}

void rpn_clear(void) {
   stack_clear();
   clear_stdout();
}

ErrorCode rpn_add(void) {
   int first = 0;
   int second = 0;
   ErrorCode err = stack_pop(&second);
   if (err != ERR_OK) {
      return err;
   }
   err = stack_pop(&first);
   if (err != ERR_OK) {
      return err;
   }
   if ((second > 0 && first > INT_MAX - second) ||
       (second < 0 && first < INT_MIN - second)) {
      return ERR_ARITH_OVERFLOW;
   }
   return stack_push(first + second);
}

ErrorCode rpn_sub(void) {
   int first = 0;
   int second = 0;
   ErrorCode err = stack_pop(&second);
   if (err != ERR_OK) {
      return err;
   }
   err = stack_pop(&first);
   if (err != ERR_OK) {
      return err;
   }
   if ((second < 0 && first > INT_MAX + second) ||
       (second > 0 && first < INT_MIN + second)) {
      return ERR_ARITH_OVERFLOW;
   }
   return stack_push(first - second);
}

ErrorCode rpn_mul(void) {
   int first = 0;
   int second = 0;
   ErrorCode err = stack_pop(&second);
   if (err != ERR_OK) {
      return err;
   }
   err = stack_pop(&first);
   if (err != ERR_OK) {
      return err;
   }

   if (first > 0) {
      // b positiv -> Grenze INT_MAX
      if (second > 0 && first > INT_MAX / second) {
         return ERR_ARITH_OVERFLOW;
      }
      // b negativ -> Grenze INT_MIN
      if (second < 0 && second < INT_MIN / first) {
         return ERR_ARITH_OVERFLOW;
      }
   } else if (first < 0) {
      // b positiv -> Grenze INT_MIN
      if (second > 0 && first < INT_MIN / second) {
         return ERR_ARITH_OVERFLOW;
      }
      // b negativ -> Minus mal Minus = Positiv -> Zeichen dreht sich um!
      if (second < 0 && first < INT_MAX / second) {
         return ERR_ARITH_OVERFLOW;
      }
   }
   return stack_push(first * second);
}

ErrorCode rpn_div(void) {
   int first = 0;
   int second = 0;
   ErrorCode err = stack_pop(&second);
   if (err != ERR_OK) {
      return err;
   }
   err = stack_pop(&first);
   if (err != ERR_OK) {
      return err;
   }
   if (second == 0) {
      return ERR_DIV_BY_ZERO;
   }
   if (first == INT_MIN && second == -1) {
      return ERR_ARITH_OVERFLOW;
   }
   return stack_push(first / second);
}
