/**
 ******************************************************************************
 * @file    colorTable.h
 * @brief   Verwaltung der 8-Bit-BMP-Farbpalette (Einlesen und Nachschlagen).
 ******************************************************************************
 */

#ifndef COLORTABLE_H
#define COLORTABLE_H

#include "BMP_types.h"
#include <stdint.h>

/**
 * @brief  Liest je nach Farbtiefe (biBitCount) die Farbpalette aus dem Stream.
 *         Bei 24-Bit-Bildern gibt es keine Palette (0 Bytes); bei 8-Bit-Bildern
 *         werden biClrUsed bzw. 256 Einträge (bei biClrUsed == 0) gelesen.
 * @param  info_hdr    Geprüfter BITMAPINFOHEADER der aktuellen Datei
 * @param  bytes_read  Ausgabe: Anzahl der gelesenen Paletten-Bytes
 * @retval EOK bei Erfolg, sonst READ_ERR oder COL_TAB_ERR
 */
int read_color_table(const BITMAPINFOHEADER *info_hdr, uint32_t *bytes_read);

/**
 * @brief  Schlägt einen 8-Bit-Palettenindex in der geladenen Farbtabelle nach
 *         und liefert die Farbe als 24-Bit-BGR-Wert (RGBTRIPLE).
 * @param  index  Palettenindex aus den Pixeldaten (0 .. num_colors - 1)
 * @param  rgb    Ausgabe: nachgeschlagener Farbwert
 * @retval EOK bei Erfolg, COL_TAB_ERR bei NULL-Zeiger, INDEX_ERR bei
 *         ungültigem Index
 */
int get_color_from_table(uint8_t index, RGBTRIPLE *rgb);

#endif /* COLORTABLE_H */
