/**
 ******************************************************************************
 * @file    lineBuffer.h
 * @brief   5-Zeilen-Puffer und Box-Filter-Mittelwertbildung für Teilaufgabe c.
 ******************************************************************************
 */

#ifndef LINEBUFFER_H
#define LINEBUFFER_H

#include "BMP_types.h"
#include <stdint.h>

/**
 * @brief  Setzt den Zeilenpuffer für ein neues Bild zurück.
 * @param  img_width  Breite einer Quellbildzeile (1 .. MAX_IMG_WIDTH)
 * @param  box_size   Kantenlänge der Kachel (1 .. MAX_SCALE_FACTOR)
 */
void line_buf_reset(uint32_t img_width, uint8_t box_size);

/**
 * @brief  Überschreibt eine Pufferzeile komplett mit Schwarz.
 * @param  slot  Zeilenindex im Puffer (0 .. MAX_SCALE_FACTOR - 1)
 */
void line_buf_clear_slot(uint8_t slot);

/**
 * @brief  Schreibt count Pixel derselben Farbe ab Spalte x_start in Zeile slot.
 * @param  slot     Zeilenindex im Puffer
 * @param  x_start  Startspalte in der Quellzeile
 * @param  rgb      Farbwert (24-Bit-BGR)
 * @param  count    Anzahl zu schreibender Pixel
 * @retval EOK bei Erfolg, sonst LINE_BUF_ERR
 */
int line_buf_put_pixels(uint8_t slot, uint32_t x_start, RGBTRIPLE rgb,
                        uint32_t count);

/**
 * @brief  Berechnet den Farb-Mittelwert einer Kachel ab Spalte x_start über
 *         valid_rows belegte Pufferzeilen (inkl. Rand- und Eckkacheln).
 * @param  x_start     Linke Startspalte der Kachel im Quellbild
 * @param  valid_rows  Anzahl gültiger Zeilen im aktuellen Streifen (1..5)
 * @retval Gemittelter Farbwert als RGBTRIPLE
 */
RGBTRIPLE line_buf_calc_box_avg(uint32_t x_start, uint8_t valid_rows);

#endif /* LINEBUFFER_H */
