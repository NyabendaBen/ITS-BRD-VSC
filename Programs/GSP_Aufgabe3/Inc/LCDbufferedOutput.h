/**
 ******************************************************************************
 * @file    LCDbufferedOutput.h
 * @brief   Kapselt die LCD-Ausgabe (RGB565-Umrechnung, Abschneiden außerhalb
 *          des Displays sowie Einzelpixel- und Zeilenausgabe).
 ******************************************************************************
 */

#ifndef LCDBUFFEREDOUTPUT_H
#define LCDBUFFEREDOUTPUT_H

#include "BMP_types.h"
#include <stdint.h>

/**
 * @brief  Bereitet die Ausgabe einer Display-Zeile an der Y-Koordinate lcd_y
 *         (0 = oben .. 319 = unten) vor und begrenzt die Breite auf LCD_WIDTH.
 * @param  lcd_y  Zielzeile auf dem Display
 * @param  width  Anzahl auszugebender Pixel in dieser Zeile
 */
void lcd_out_start_line(uint16_t lcd_y, uint16_t width);

/**
 * @brief  Rechnet ein 24-Bit-BGR-Pixel in 16-Bit RGB565 um und zeichnet es
 *         entweder sofort (Teil a) oder legt es im Zeilenpuffer ab (Teil b/c).
 *         Pixel außerhalb des Displays werden ignoriert.
 * @param  lcd_x  Spalte auf dem Display (0 .. 479)
 * @param  rgb    24-Bit-Farbwert des Pixels
 */
void lcd_out_put_pixel(uint16_t lcd_x, RGBTRIPLE rgb);

/**
 * @brief  Gibt in Teilaufgabe b/c die gepufferte Display-Zeile über
 *         GUI_WriteLine() am Stück aus (in Teilaufgabe a ohne Wirkung).
 */
void lcd_out_flush_line(void);

#endif /* LCDBUFFEREDOUTPUT_H */
