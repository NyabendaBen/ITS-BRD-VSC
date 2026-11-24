/**
 ******************************************************************************
 * @file    LCDbufferedOutput.c
 * @brief   Wandelt 24-Bit-BGR-Pixel in 16-Bit RGB565 um und gibt sie einzeln
 *          (Teil a) oder zeilenweise gepuffert (Teil b/c) auf dem LCD aus.
 ******************************************************************************
 */

#include "LCDbufferedOutput.h"
#include "BMP_types.h"
#include "LCD_GUI.h"
#include "LCD_general.h"
#include "bmp_config.h"
#include <stdint.h>

/* Bit-Shifts und Masken für 24-Bit-Farben -> 16-Bit RGB565 (5/6/5 Bit) */
#define RED_SHIFT_RIGHT 3U
#define GREEN_SHIFT_RIGHT 2U
#define BLUE_SHIFT_RIGHT 3U

#define RED_SHIFT_LEFT 11U
#define GREEN_SHIFT_LEFT 5U

#define BITMASK_BIT_5 0x1FU
#define BITMASK_BIT_6 0x3FU

/* Zeilenpuffer für Teilaufgabe b/c in .bss (960 Bytes, nicht auf dem Stack) */
#if (USE_LINE_BUFFER_OUTPUT != 0)
static COLOR lcd_line_buf[LCD_WIDTH];
#endif

static uint16_t current_lcd_y = 0U;
static uint16_t current_line_width = 0U;

/* Rechnet 8-Bit-Kanäle (BGR) auf die oberen Bits für 16-Bit RGB565 um */
static COLOR rgb_to_lcd_color(RGBTRIPLE rgb) {
   uint32_t red = ((uint32_t)rgb.rgbtRed >> RED_SHIFT_RIGHT) & BITMASK_BIT_5;
   uint32_t green =
       ((uint32_t)rgb.rgbtGreen >> GREEN_SHIFT_RIGHT) & BITMASK_BIT_6;
   uint32_t blue = ((uint32_t)rgb.rgbtBlue >> BLUE_SHIFT_RIGHT) & BITMASK_BIT_5;

   return (COLOR)((red << RED_SHIFT_LEFT) | (green << GREEN_SHIFT_LEFT) | blue);
}

void lcd_out_start_line(uint16_t lcd_y, uint16_t width) {
   current_lcd_y = lcd_y;

   /* Breite auf die maximale Displaybreite (480) begrenzen */
   if (width > (uint16_t)LCD_WIDTH) {
      current_line_width = (uint16_t)LCD_WIDTH;
   } else {
      current_line_width = width;
   }
}

void lcd_out_put_pixel(uint16_t lcd_x, RGBTRIPLE rgb) {
   /* Pixel außerhalb des Displays nicht zeichnen */
   if ((current_lcd_y >= (uint16_t)LCD_HEIGHT) ||
       (lcd_x >= current_line_width)) {
      return;
   }

   COLOR color = rgb_to_lcd_color(rgb);

#if (USE_LINE_BUFFER_OUTPUT == 0)
   /* Teilaufgabe a: jedes Pixel einzeln zeichnen */
   Coordinate top_left = {lcd_x, current_lcd_y};
   GUI_drawPoint(top_left, color, DOT_PIXEL_1X1, DOT_FILL_AROUND);
#else
   /* Teilaufgabe b/c: Pixel im Zeilenpuffer sammeln */
   lcd_line_buf[lcd_x] = color;
#endif
}

void lcd_out_flush_line(void) {
#if (USE_LINE_BUFFER_OUTPUT != 0)
   /* Ganze Display-Zeile in einem SPI-Burst ausgeben */
   if ((current_lcd_y < (uint16_t)LCD_HEIGHT) && (current_line_width > 0U)) {
      Coordinate top_left = {0U, current_lcd_y};
      GUI_WriteLine(top_left, current_line_width, lcd_line_buf);
   }
#endif
}
