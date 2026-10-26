#ifndef RPN_H
#define RPN_H

#include "errorhandler.h"

// Zahl auf den Stack legen
ErrorCode rpn_number(int value);

// 'p' den obersten Wert anzeigen
ErrorCode rpn_print_top(void);

// 'P' die drei obersten Werte anzeigen
ErrorCode rpn_print_three(void);

// 'd' obersten Wert duplizieren
ErrorCode rpn_duplicate(void);

// 'r' die beiden obersten Werte vertauschen
ErrorCode rpn_swap(void);

// Arithmetische Operationen
ErrorCode rpn_add(void);
ErrorCode rpn_sub(void);
ErrorCode rpn_mul(void);
ErrorCode rpn_div(void);

// 'C' Stack und Display zurücksetzen
void rpn_clear(void);

#endif // RPN_H