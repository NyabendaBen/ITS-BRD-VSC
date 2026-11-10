#ifndef INC_CALC_H_
#define INC_CALC_H_

#include "encoder.h"
#include <stdbool.h>

// Berechnete Messwerte
typedef struct {
   double angle_deg;
   double speed_deg_per_s;
} Measurement;

/**
 * @brief Synchronisiert die Berechnungseinheit nach Systemstart oder S6 Reset.
 * @param[in] start Startwert des Phasenzählers.
 * @param[in] now Aktueller Zeitstempel.
 */
void calc_reset(PhaseCounter start, uint64_t now);

/**
 * @brief Berechnet Winkel und Geschwindigkeit mit 300ms- und 3s-Fenster.
 * @param[in] counter Zeiger auf den aktuellen Phasenzähler.
 * @param[in] now Aktueller Zeitstempel des Loop Durchlaufs.
 * @param[out] result Zeiger auf die Struktur für die berechneten Werte.
 * @return True wenn neue Werte berechnet wurden, sonst False.
 */
bool calc_update(const PhaseCounter *counter, uint64_t now,
                 Measurement *result);

#endif /* INC_CALC_H_ */