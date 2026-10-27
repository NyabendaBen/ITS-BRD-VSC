#ifndef STACK_H
#define STACK_H

#include "errorhandler.h"

#define STACK_SIZE 8

/**
 * @brief Legt einen ganzzahligen Wert oben auf den Stack ab.
 * @note Aufrufer: rpn-Modul (rpn_number, rpn_duplicate, rpn_swap, Arithmetik).
 * @param[in] value Der abzulegende Wert.
 * @return ERR_OK bei Erfolg, ERR_STACK_OVERFLOW wenn der Stack voll ist.
 */
ErrorCode stack_push(int value);

/**
 * @brief Entfernt den obersten Wert vom Stack und liefert ihn zurueck.
 * @note Aufrufer: rpn-Modul (rpn_swap, rpn_add, rpn_sub, rpn_mul, rpn_div).
 * @param[out] value Zeiger auf die Variable fuer den entnommenen Wert.
 * @return ERR_OK bei Erfolg, ERR_STACK_UNDERFLOW wenn der Stack leer ist.
 */
ErrorCode stack_pop(int *value);

/**
 * @brief Liest einen Wert an relativer Tiefe, ohne ihn vom Stack zu entfernen.
 * @note Aufrufer: rpn-Modul (rpn_print_top, rpn_print_three, rpn_duplicate).
 * @param[in] depth Relative Tiefe ab Stack-Spitze (0 = oberstes Element).
 * @param[out] value Zeiger auf die Variable fuer den gelesenen Wert.
 * @return ERR_OK bei Erfolg, ERR_STACK_UNDERFLOW bei ungueltiger Tiefe.
 */
ErrorCode stack_peek(int depth, int *value);

/**
 * @brief Gibt die Anzahl der aktuell gespeicherten Elemente zurueck.
 * @note Aufrufer: rpn-Modul (rpn_print_three).
 * @return Elementanzahl (0 bis STACK_SIZE).
 */
int stack_size(void);

/**
 * @brief Leert den Stack vollstaendig.
 * @note Aufrufer: rpn-Modul (rpn_clear).
 */
void stack_clear(void);

#endif /* STACK_H */
