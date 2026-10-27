#ifndef RPN_H
#define RPN_H

#include "errorhandler.h"

#define PRINT_SIZE 16
#define PRINT_THREE_COUNT 3

/**
 * @brief Legt eine eingegebene Zahl auf den Stack.
 * @note Aufrufer: execute() in main.c (Token NUMBER).
 * @param[in] value Der abzulegende Zahlenwert.
 * @return ERR_OK bei Erfolg, ERR_STACK_OVERFLOW bei vollem Stack.
 */
ErrorCode rpn_number(int value);

/**
 * @brief Gibt den obersten Stack Wert auf dem Display aus ('p').
 * @note Aufrufer: execute() in main.c (Token PRT).
 * @return ERR_OK bei Erfolg, ERR_STACK_UNDERFLOW wenn der Stack leer ist.
 */
ErrorCode rpn_print_top(void);

/**
 * @brief Gibt die bis zu drei obersten Stack Werte auf dem Display aus ('P').
 * @note Aufrufer: execute() in main.c (Token PRT_ALL).
 * @return ERR_OK bei Erfolg, ERR_STACK_UNDERFLOW nur wenn der Stack
 * vollständig leer ist.
 */
ErrorCode rpn_print_three(void);

/**
 * @brief Dupliziert den obersten Stack Wert ('d').
 * @note Aufrufer: execute() in main.c (Token DOUBLE).
 * @return ERR_OK bei Erfolg, ERR_STACK_UNDERFLOW (leer) oder ERR_STACK_OVERFLOW
 * (voll).
 */
ErrorCode rpn_duplicate(void);

/**
 * @brief Vertauscht die beiden obersten Stack Werte ('r').
 * @note Aufrufer: execute() in main.c (Token SWAP).
 * @return ERR_OK bei Erfolg, ERR_STACK_UNDERFLOW wenn weniger als 2 Elemente
 * vorhanden.
 */
ErrorCode rpn_swap(void);

/**
 * @brief Addiert die beiden obersten Stack Werte mit Überlaufprüfung ('+').
 * @note Aufrufer: execute() in main.c (Token PLUS).
 * @return ERR_OK bei Erfolg, ERR_STACK_UNDERFLOW (< 2 Werte) oder
 * ERR_ARITH_OVERFLOW.
 */
ErrorCode rpn_add(void);

/**
 * @brief Subtrahiert den obersten vom zweitobersten Stack Wert ('-').
 * @note Aufrufer: execute() in main.c (Token MINUS).
 * @return ERR_OK bei Erfolg, ERR_STACK_UNDERFLOW (< 2 Werte) oder
 * ERR_ARITH_OVERFLOW.
 */
ErrorCode rpn_sub(void);

/**
 * @brief Multipliziert die beiden obersten Stack Werte mit Überlaufprüfung
 * ('*').
 * @note Aufrufer: execute() in main.c (Token MULT).
 * @return ERR_OK bei Erfolg, ERR_STACK_UNDERFLOW (< 2 Werte) oder
 * ERR_ARITH_OVERFLOW.
 */
ErrorCode rpn_mul(void);

/**
 * @brief Dividiert den zweitobersten durch den obersten Stack Wert ganzzahlig
 * ('/').
 * @note Aufrufer: execute() in main.c (Token DIV).
 * @return ERR_OK, ERR_STACK_UNDERFLOW, ERR_DIV_BY_ZERO oder ERR_ARITH_OVERFLOW.
 */
ErrorCode rpn_div(void);

/**
 * @brief Setzt Stack und Display Terminal zurück ('C').
 * @note Aufrufer: main.c (Token CLEAR).
 */
void rpn_clear(void);

#endif /* RPN_H */