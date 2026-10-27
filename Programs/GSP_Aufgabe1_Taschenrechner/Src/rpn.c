#include "rpn.h"
#include "display.h"
#include "stack.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

// Hilfsfunktion: Wandelt eine Zahl in Text um und schreibt sie ins Terminal
static void print_number(int value) {
   char buf[PRINT_SIZE];
   (void)snprintf(buf, sizeof(buf), "%d\n", value);
   print_stdout(buf);
}

// Zifferneingabe: Zahl auf den Stack legen
ErrorCode rpn_number(int value) { return stack_push(value); }

// Taste 'p': Obersten Wert lesen und ausgeben, ohne ihn zu loeschen
ErrorCode rpn_print_top(void) {
   int value = 0;
   ErrorCode err = stack_peek(0, &value);
   if (err != ERR_OK) {
      return err;
   }
   print_number(value);
   return ERR_OK;
}

// Taste 'P': Bis zu 3 oberste Werte in einer Zeile mit Leerzeichen ausgeben
ErrorCode rpn_print_three(void) {
   int current_size = stack_size();
   int limit = PRINT_THREE_COUNT;
   // Wenn Stack komplett leer ist -> Unterlauf
   if (current_size == 0) {
      return ERR_STACK_UNDERFLOW;
   }
   // Falls weniger als 3 Zahlen da sind (z. B. 1 oder 2), geben wir nur die aus
   if (current_size < limit) {
      limit = current_size;
   }
   char buf[PRINT_SIZE];
   for (int depth = 0; depth < limit; depth++) {
      int value = 0;
      (void)stack_peek(depth, &value);
      (void)snprintf(buf, sizeof(buf), "%d ", value);
      print_stdout(buf);
   }
   print_stdout("\n");
   return ERR_OK;
}

// Taste 'd': Oberste Zahl duplizieren (einmal lesen und gleich nochmal pushen)
ErrorCode rpn_duplicate(void) {
   int value = 0;
   ErrorCode err = stack_peek(0, &value);
   if (err != ERR_OK) {
      return err;
   }
   return stack_push(value);
}

// Taste 'r': Die beiden obersten Zahlen auf dem Stack vertauschen
ErrorCode rpn_swap(void) {
   int top = 0;
   int bottom = 0;
   // Beide Werte nacheinander vom Stack holen
   ErrorCode err = stack_pop(&top);
   if (err != ERR_OK) {
      return err;
   }
   err = stack_pop(&bottom);
   if (err != ERR_OK) {
      return err;
   }
   // In umgekehrter Reihenfolge wieder ablegen
   (void)stack_push(top);
   (void)stack_push(bottom);
   return ERR_OK;
}

// Taste 'C': Alles zurücksetzen – Stack leeren und oberes Terminal
void rpn_clear(void) {
   stack_clear();
   clear_stdout();
}

// Taste '+': Addition zweier Werte (first + second)
ErrorCode rpn_add(void) {
   int first = 0;
   int second = 0;
   // LIFO: Erstes pop liefert den oberen Operanden (second), zweites den
   // unteren (first)
   ErrorCode err = stack_pop(&second);
   if (err != ERR_OK) {
      return err;
   }
   err = stack_pop(&first);
   if (err != ERR_OK) {
      return err;
   }
   // Vorab prüfen: Würde die Summe über INT_MAX oder unter INT_MIN laufen?
   if ((second > 0 && first > INT_MAX - second) ||
       (second < 0 && first < INT_MIN - second)) {
      return ERR_ARITH_OVERFLOW;
   }
   return stack_push(first + second);
}

// Taste '-': Subtraktion (zweiter minus oberster Operand: first - second)
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
   // Vorab auf Über- oder Unterlauf prüfen
   if ((second < 0 && first > INT_MAX + second) ||
       (second > 0 && first < INT_MIN + second)) {
      return ERR_ARITH_OVERFLOW;
   }
   return stack_push(first - second);
}

// Taste '*': Multiplikation zweier Werte (first * second)
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

   // Vorab alle 4 Vorzeichen-Kombinationen gegen INT_MAX und INT_MIN absichern:
   if (first > 0) {
      // Plus mal Plus -> darf nicht groesser als INT_MAX werden
      if (second > 0 && first > INT_MAX / second) {
         return ERR_ARITH_OVERFLOW;
      }
      // Plus mal Minus -> darf nicht kleiner als INT_MIN werden
      if (second < 0 && second < INT_MIN / first) {
         return ERR_ARITH_OVERFLOW;
      }
   } else if (first < 0) {
      // Minus mal Plus -> darf ebenfalls nicht kleiner als INT_MIN werden
      if (second > 0 && first < INT_MIN / second) {
         return ERR_ARITH_OVERFLOW;
      }
      // Minus mal Minus ergibt Plus -> geteilt durch negatives second dreht das
      // Zeichen um
      if (second < 0 && first < INT_MAX / second) {
         return ERR_ARITH_OVERFLOW;
      }
   }
   return stack_push(first * second);
}

// Taste '/': Ganzzahlige Division (first / second)
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
   // 1. Division durch 0 abfangen
   if (second == 0) {
      return ERR_DIV_BY_ZERO;
   }
   // 2. Sonderfall des Zweierkomplements: INT_MIN / -1 ergibt +2147483648 ->
   // Überlauf
   if (first == INT_MIN && second == -1) {
      return ERR_ARITH_OVERFLOW;
   }
   return stack_push(first / second);
}
