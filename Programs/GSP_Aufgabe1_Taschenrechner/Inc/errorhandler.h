#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

typedef enum {
   ERR_OK = 0,
   ERR_DIV_BY_ZERO,
   ERR_ARITH_OVERFLOW,
   ERR_INPUT_OVERFLOW,
   ERR_STACK_OVERFLOW,
   ERR_STACK_UNDERFLOW,
   ERR_UNEXPECTED_INPUT,
} ErrorCode;

const char *error_text(ErrorCode err);

#endif // ERRORHANDLER_H