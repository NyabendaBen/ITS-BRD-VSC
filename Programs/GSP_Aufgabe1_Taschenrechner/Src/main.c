/**
 ******************************************************************************
 * @file    main.c
 * @author  Franz Korf
 * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
 ******************************************************************************
 */
#include "main.h"
#include "display.h"
#include "errorhandler.h"
#include "init.h"
#include "rpn.h"
#include "scanner.h"
#include "token.h"
#include <stdio.h>
#include <stdbool.h>

static ErrorCode execute(T_token token) {
   switch (token.tok) {
   case NUMBER:
      return rpn_number(token.val);
   case PLUS:
      return rpn_add();
   case MINUS:
      return rpn_sub();
   case MULT:
      return rpn_mul();
   case DIV:
      return rpn_div();
   case PRT:
      return rpn_print_top();
   case PRT_ALL:
      return rpn_print_three();
   case DOUBLE:
      return rpn_duplicate();
   case SWAP:
      return rpn_swap();
   case OVERFLOW:
      return ERR_INPUT_OVERFLOW;
   default:
      // Für # oder e
      return ERR_UNEXPECTED_INPUT;
   }
}

static void print_error_text(ErrorCode err) {
   char buf[ERR_TEXT_SIZE];
   (void)snprintf(buf, sizeof(buf), "%s\n", error_text(err));
   print_stdout(buf);
}

int main(void) {

   initITSboard();
   init_display();

   bool error_mode = false;

   while (1) {
      T_token token = next_token();

      if (token.tok == CLEAR) {
         rpn_clear();
         set_normal_mode();
         error_mode = false;
         continue;
      }

      if (error_mode) {
         continue; // Ignoriere Eingaben, wenn ein Fehler aufgetreten ist
      }
      ErrorCode err = execute(token);

      if (err != ERR_OK) {
         set_err_mode();
         print_error_text(err);
         error_mode =
             true; // Setze den Fehlerstatus, um weitere Eingaben zu ignorieren
      }
   }
}

// EOF
