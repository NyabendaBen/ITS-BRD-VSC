#ifndef DISPLAY2_H_
#define DISPLAY2_H_

#include "calc.h"
#include <stdbool.h>

/**
 * @brief Initialisiert das LCD und druckt die festen Beschriftungstexte.
 */
void display_init(void);

/**
 * @brief Schreibt neue Messwerte in den internen Ausgabepuffer.
 * @param[in] meas Zeiger auf die anzuzeigenden Messwerte.
 */
void display_request(const Measurement *meas);

/**
 * @brief Gibt maximal 1 Zeichen aus dem Puffer aufs LCD aus.
 */
void display_process(void);

/**
 * @brief Prüft, ob noch Zeichen im Puffer auf Ausgabe warten.
 * @return True wenn Ausgabe noch beschäftigt ist, sonst False.
 */
bool display_is_busy(void);

#endif /* DISPLAY2_H_ */
