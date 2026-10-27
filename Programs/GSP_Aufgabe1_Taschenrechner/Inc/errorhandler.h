#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

/**
 * @brief Einheitliche Fehlercodes des RPN-Taschenrechners.
 */
typedef enum {
   ERR_OK = 0,           // Kein Fehler
   ERR_DIV_BY_ZERO,      // Division durch 0
   ERR_ARITH_OVERFLOW,   // Ueberlauf bei + - * /
   ERR_INPUT_OVERFLOW,   // Zahl groesser als INT_MAX
   ERR_STACK_OVERFLOW,   // Stack voll
   ERR_STACK_UNDERFLOW,  // Zu wenige Werte auf dem Stack
   ERR_UNEXPECTED_INPUT, // Unerwartetes Token ('#' oder 'e')
} ErrorCode;

/**
 * @brief Liefert den lesbaren Fehlertext zu einem Fehlercode.
 * @note Aufrufer: print_error_text() in main.c.
 * @param[in] err Fehlercode vom Typ ErrorCode.
 * @return Zeiger auf den Fehlertext als String-Literal.
 */
const char *error_text(ErrorCode err);

#endif /* ERRORHANDLER_H */
