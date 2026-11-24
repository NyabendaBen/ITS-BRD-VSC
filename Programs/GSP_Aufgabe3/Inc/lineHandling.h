/**
 ******************************************************************************
 * @file    lineHandling.h
 * @brief   Koordiniert Zeilenpufferung, Verkleinerung (Faktor 1..5),
 *          Bottom-Up-Umrechnung und die Ausgabe auf das LCD.
 ******************************************************************************
 */

#ifndef LINEHANDLING_H
#define LINEHANDLING_H

#include "BMP_types.h"
#include <stdint.h>

/**
 * @brief  Initialisiert die Zeilenverarbeitung für ein neues Bild, berechnet
 *         den Skalierungsfaktor (box_size 1..5) und leert den Zeilenpuffer.
 * @param  width   Bildbreite in Pixeln (biWidth)
 * @param  height  Bildhöhe in Pixeln (biHeight)
 * @retval EOK bei Erfolg, sonst IMG_SIZE_ERR
 */
int init_line_handling(uint32_t width, uint32_t height);

/**
 * @brief  Schreibt count aufeinanderfolgende Pixel derselben Farbe ab der
 *         aktuellen X-Position in die laufende Zeile und prüft die Bildgrenzen.
 * @param  rgb    Farbwert (24-Bit-BGR)
 * @param  count  Anzahl der Pixel
 * @retval EOK bei Erfolg, sonst FORMAT_ERR oder LINE_BUF_ERR
 */
int put_pixel_run(RGBTRIPLE rgb, uint32_t count);

/**
 * @brief  Schließt die aktuelle Bildzeile ab (füllt fehlende Pixel ggf. mit
 *         Schwarz auf) und gibt bei vollem Streifen die gemittelte Zeile aus.
 * @retval EOK bei Erfolg, sonst FORMAT_ERR oder LINE_BUF_ERR
 */
int finish_current_line(void);

/**
 * @brief  Führt einen RLE-Delta-Sprung (00 02) um (delta_x, delta_y) aus und
 *         füllt übersprungene Pixel und Zeilen mit Schwarz auf.
 * @param  delta_x  Verschiebung nach rechts (0..255)
 * @param  delta_y  Verschiebung nach oben im Bottom-Up-Bild (0..255)
 * @retval EOK bei Erfolg, sonst FORMAT_ERR oder LINE_BUF_ERR
 */
int apply_delta(uint8_t delta_x, uint8_t delta_y);

/**
 * @brief  Gibt am Bildende (EOB) eine angefangene Zeile sowie einen noch nicht
 *         ausgegebenen Rest-Streifen am oberen Bildrand auf dem LCD aus.
 * @retval EOK bei Erfolg, sonst Fehlercode aus finish_current_line()
 */
int flush_remaining_lines(void);

#endif /* LINEHANDLING_H */
