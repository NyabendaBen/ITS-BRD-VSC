#include "main.h"
#include "display.h"
#include "errorhandler.h"
#include "init.h"
#include "rpn.h"
#include "scanner.h"
#include "token.h"
#include <stdbool.h>
#include <stdio.h>

// Schaut nach, welche Taste gedrueckt wurde, und ruft die passende Funktion auf
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
      // Scanner meldet: Zahl > INT_MAX eingetippt
      return ERR_INPUT_OVERFLOW;
   default:
      // Tasten '#' und 'e' erzeugen das Token UNEXPECTED
      return ERR_UNEXPECTED_INPUT;
   }
}

// Fehlertext formatieren und ins Terminal schreiben
static void print_error_text(ErrorCode err) {
   char buf[ERR_TEXT_SIZE];
   (void)snprintf(buf, sizeof(buf), "%s\n", error_text(err));
   print_stdout(buf);
}

int main(void) {
   // Hardware und Touch-Display initialisieren
   initITSboard();
   init_display();

   bool error_mode = false;

   // Endlosschleife: Auf Touch-Eingaben warten
   while (1) {
      T_token token = next_token();

      // CLEAR setzt den Rechner jederzeit zurueck
      if (token.tok == CLEAR) {
         rpn_clear();
         set_normal_mode();
         error_mode = false;
         continue;
      }

      // Solange ein Fehler vorliegt: alle Tasten ausser CLEAR ignorieren
      if (error_mode) {
         continue;
      }

      // Eingegebenen Befehl ausfuehren
      ErrorCode err = execute(token);

      // Bei Fehler: Terminal auf rot schalten und Fehlermodus aktivieren
      if (err != ERR_OK) {
         set_err_mode();
         print_error_text(err);
         error_mode = true;
      }
   }
}
