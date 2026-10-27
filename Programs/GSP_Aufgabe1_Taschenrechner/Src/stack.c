
#include "stack.h"
#include "errorhandler.h"

// Speicher fuer maximal 8 Zahlen; count merkt sich wie viele gerade drauf
// liegen
static int data[STACK_SIZE];
static int count = 0;

// Liefert die Anzahl der aktuell gespeicherten Elemente
int stack_size(void) { return count; }

// Stack leeren: count auf 0 setzen
void stack_clear(void) { count = 0; }

// Neue Zahl oben auf den Stack legen
ErrorCode stack_push(int value) {
   // Wenn der Stack voll ist (8 Zahlen), Fehler melden
   if (count >= STACK_SIZE) {
      return ERR_STACK_OVERFLOW;
   }
   // Wert an der naechsten freien Position speichern und Zaehler erhoehen
   data[count] = value;
   count++;
   return ERR_OK;
}

// Holt den obersten Wert vom Stack (Call-by-Reference ueber Zeiger)
ErrorCode stack_pop(int *value) {
   // Pruefen, ob mindestens ein Element vorhanden ist
   if (count <= 0) {
      return ERR_STACK_UNDERFLOW;
   }
   // Zaehler verringern und obersten Wert ueber den Zeiger zurueckgeben
   count--;
   *value = data[count];
   return ERR_OK;
}

// Liest einen Wert relativ zur Spitze (depth 0 = oberstes Element), ohne ihn zu
// entfernen
ErrorCode stack_peek(int depth, int *value) {
   // Pruefen, ob die gewuenschte Tiefe gueltig ist
   if (depth < 0 || depth >= count) {
      return ERR_STACK_UNDERFLOW;
   }
   // Von oben nach unten zaehlen: count - 1 ist oben, - depth geht weiter nach
   // unten
   *value = data[count - 1 - depth];
   return ERR_OK;
}
