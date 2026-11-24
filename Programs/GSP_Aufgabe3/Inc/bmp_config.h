/**
 ******************************************************************************
 * @file    bmp_config.h
 * @brief   Zentrale Einstellungen für die BMP-Bildanzeige (Teilaufgaben a–c).
 ******************************************************************************
 */

#ifndef BMP_CONFIG_H
#define BMP_CONFIG_H

#include "LCD_general.h" /* LCD_WIDTH (480), LCD_HEIGHT (320) */

/* Ausgabeart: 0 = Einzelpixel (Teil a), 1 = Zeilenpuffer (Teil b & c) */
#define USE_LINE_BUFFER_OUTPUT 1

/* Skalierung: 0 = kein Verkleinern / Abschneiden (Teil a & b),
 *             1 = Box-Filter bis 1/5 (Teil c) */
#define ENABLE_DOWNSCALING 1

/* Maximaler Verkleinerungsfaktor (1/5) und maximale unterstützte Bildgröße */
#define MAX_SCALE_FACTOR 5U
#define MAX_IMG_WIDTH (MAX_SCALE_FACTOR * (uint32_t)LCD_WIDTH)   /* 2400 */
#define MAX_IMG_HEIGHT (MAX_SCALE_FACTOR * (uint32_t)LCD_HEIGHT) /* 1600 */

#endif /* BMP_CONFIG_H */
