/**
 ******************************************************************************
 * @file    lineHandling.c
 * @brief   Verwaltet Bildgeometrie, Skalierungsfaktor (box_size 1..5),
 *          Bottom-Up-Umrechnung und RLE-Delta-Sprünge.
 ******************************************************************************
 */

#include "lineHandling.h"
#include "BMP_types.h"
#include "LCD_general.h"
#include "LCDbufferedOutput.h"
#include "bmp_config.h"
#include "errorhandler.h"
#include "lineBuffer.h"
#include <stdint.h>

/* Abmessungen des Quellbildes und des skalierten Ausgabebildes */
static uint32_t img_width = 0U;
static uint32_t img_height = 0U;
static uint32_t out_width = 0U;
static uint32_t out_height = 0U;

/* Aktuelle Schreibposition im Quellbild und Zähler der Ausgabestreifen */
static uint32_t current_x = 0U;
static uint32_t current_y = 0U;
static uint32_t strip_idx = 0U;

/* Kachelgröße (1..5) und Anzahl aktuell gesammelter Zeilen im Puffer */
static uint8_t box_size = 1U;
static uint8_t rows_in_buf = 0U;

static const RGBTRIPLE BLACK_RGB = {0U, 0U, 0U};

/* Mittelt die gesammelten Pufferzeilen zu einer LCD-Zeile und gibt sie aus */
static void flush_strip_to_lcd(uint8_t valid_rows) {
   if ((valid_rows == 0U) || (strip_idx >= out_height)) {
      return;
   }

   /* BMP ist Bottom-Up, das LCD Top-Down -> erster Streifen liegt unten */
   uint32_t lcd_y = (out_height - 1U) - strip_idx;

   if (lcd_y < (uint32_t)LCD_HEIGHT) {
      uint16_t vis_width = (out_width > (uint32_t)LCD_WIDTH)
                               ? (uint16_t)LCD_WIDTH
                               : (uint16_t)out_width;

      lcd_out_start_line((uint16_t)lcd_y, vis_width);
      for (uint16_t dst_x = 0U; dst_x < vis_width; dst_x++) {
         RGBTRIPLE avg = line_buf_calc_box_avg(
             (uint32_t)dst_x * (uint32_t)box_size, valid_rows);
         lcd_out_put_pixel(dst_x, avg);
      }
      lcd_out_flush_line();
   }

   /* Immer weiterzählen, damit die nächste Zeile um 1 nach oben wandert */
   strip_idx++;
}

int init_line_handling(uint32_t width, uint32_t height) {
   CHECK_OR_RETURN((width >= 1U) && (width <= MAX_IMG_WIDTH), IMG_SIZE_ERR);
   CHECK_OR_RETURN((height >= 1U) && (height <= MAX_IMG_HEIGHT), IMG_SIZE_ERR);

   uint32_t calc_box = 1U;
#if (ENABLE_DOWNSCALING != 0)
   /* Ganzzahlig aufrunden: ceil(Breite / 480) und ceil(Höhe / 320) */
   uint32_t scale_x = (width + (uint32_t)LCD_WIDTH - 1U) / (uint32_t)LCD_WIDTH;
   uint32_t scale_y =
       (height + (uint32_t)LCD_HEIGHT - 1U) / (uint32_t)LCD_HEIGHT;
   /* Größeren Faktor wählen, damit das Bild nicht verzerrt wird */
   calc_box = (scale_x > scale_y) ? scale_x : scale_y;
#endif

   CHECK_OR_RETURN((calc_box >= 1U) && (calc_box <= MAX_SCALE_FACTOR),
                   IMG_SIZE_ERR);

   img_width = width;
   img_height = height;
   box_size = (uint8_t)calc_box;
   out_width = (img_width + (uint32_t)box_size - 1U) / (uint32_t)box_size;
   out_height = (img_height + (uint32_t)box_size - 1U) / (uint32_t)box_size;

   current_x = 0U;
   current_y = 0U;
   rows_in_buf = 0U;
   strip_idx = 0U;

   line_buf_reset(width, box_size);
   line_buf_clear_slot(0U);
   return EOK;
}

int put_pixel_run(RGBTRIPLE rgb, uint32_t count) {
   /* Prüfen, ob noch Platz in der aktuellen Zeile und im Bild ist */
   CHECK_OR_RETURN(current_y < img_height, FORMAT_ERR);
   CHECK_OR_RETURN(count <= (img_width - current_x), FORMAT_ERR);

   RETURN_ON_ERROR(line_buf_put_pixels(rows_in_buf, current_x, rgb, count));
   current_x += count;
   return EOK;
}

int finish_current_line(void) {
   CHECK_OR_RETURN(current_y < img_height, FORMAT_ERR);

   /* Falls die Zeile kürzer als img_width war, den Rest schwarz auffüllen */
   if (current_x < img_width) {
      RETURN_ON_ERROR(line_buf_put_pixels(rows_in_buf, current_x, BLACK_RGB,
                                          img_width - current_x));
   }

   current_x = 0U;
   current_y++;
   rows_in_buf++;

   /* Sobald box_size Zeilen voll sind, gemittelten Streifen aufs LCD senden */
   if (rows_in_buf == box_size) {
      flush_strip_to_lcd(rows_in_buf);
      rows_in_buf = 0U;
   }

   /* Nächste Pufferzeile für neue Pixel bereinigen */
   if (current_y < img_height) {
      line_buf_clear_slot(rows_in_buf);
   }

   return EOK;
}

/* Füllt count schwarze Pixel ab der aktuellen Position auf */
static int skip_black_pixels(uint32_t count) {
   if (count == 0U) {
      return EOK;
   }
   return put_pixel_run(BLACK_RGB, count);
}

/* Schließt die laufende Zeile ab und überspringt delta_y Zeilen nach oben */
static int advance_delta_lines(uint8_t delta_y) {
   for (uint32_t step = 0U; step < (uint32_t)delta_y; step++) {
      RETURN_ON_ERROR(finish_current_line());
   }
   return EOK;
}

int apply_delta(uint8_t delta_x, uint8_t delta_y) {
   /* Zielposition sichern, da finish_current_line() current_x auf 0 setzt */
   uint32_t target_x = current_x + (uint32_t)delta_x;
   uint32_t target_y = current_y + (uint32_t)delta_y;

   CHECK_OR_RETURN(target_x <= img_width, FORMAT_ERR);
   CHECK_OR_RETURN(target_y < img_height, FORMAT_ERR);

   if (delta_y == 0U) {
      return skip_black_pixels((uint32_t)delta_x);
   }

   RETURN_ON_ERROR(advance_delta_lines(delta_y));
   return skip_black_pixels(target_x);
}

int flush_remaining_lines(void) {
   /* Noch offene letzte Zeile abschließen (wenn kein EOL vor EOB kam) */
   if ((current_x > 0U) && (current_y < img_height)) {
      RETURN_ON_ERROR(finish_current_line());
   }

   /* Übrig gebliebenen Rest-Streifen am oberen Bildrand ausgeben */
   if (rows_in_buf > 0U) {
      flush_strip_to_lcd(rows_in_buf);
      rows_in_buf = 0U;
   }

   return EOK;
}
