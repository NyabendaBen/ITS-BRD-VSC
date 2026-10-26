#include "errorhandler.h"

const char *error_text(ErrorCode err) {
   switch (err) {
   case ERR_OK:
      return "OK";
   case ERR_DIV_BY_ZERO:
      return "Division durch 0";
   case ERR_ARITH_OVERFLOW:
      return "Arithmetischer Ueberlauf";
   case ERR_INPUT_OVERFLOW:
      return "Eingabe Ueberlauf";
   case ERR_STACK_OVERFLOW:
      return "Stack Ueberlauf";
   case ERR_STACK_UNDERFLOW:
      return "Stack Unterlauf";
   case ERR_UNEXPECTED_INPUT:
      return "Unerwartete Eingabe";
   default:
      return "Unbekannter Fehler";
   }
}
