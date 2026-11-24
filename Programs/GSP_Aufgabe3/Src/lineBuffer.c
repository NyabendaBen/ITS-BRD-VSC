/**
 ******************************************************************************
 * @file    lineBuffer.c
 * @brief   Hält bis zu 5 Quellzeilen im statischen RAM und berechnet den
 *          Farb-Mittelwert einer Kachel für den Box-Filter (Teilaufgabe c).
 ******************************************************************************
 */

#include "lineBuffer.h"
#include "BMP_types.h"
#include "bmp_config.h"
#include "errorhandler.h"
#include <stdint.h>

/* 5-Zeilen-Puffer in .bss (5 x 2400 x 3 = 36 000 Bytes) */
static RGBTRIPLE line_buf[MAX_SCALE_FACTOR][MAX_IMG_WIDTH];
static uint32_t buf_img_width = 0U;
static uint8_t buf_box_size = 1U;

void line_buf_reset(uint32_t img_width, uint8_t box_size) {
   CHECK_FATAL(img_width >= 1U && img_width <= MAX_IMG_WIDTH, LINE_BUF_ERR);
   CHECK_FATAL(box_size >= 1U && (uint32_t)box_size <= MAX_SCALE_FACTOR,
               LINE_BUF_ERR);

   buf_img_width = img_width;
   buf_box_size = box_size;
}

void line_buf_clear_slot(uint8_t slot) {
   if ((uint32_t)slot >= MAX_SCALE_FACTOR) {
      return;
   }

   /* Alte Pixel aus dem vorherigen Streifen mit Schwarz überschreiben */
   for (uint32_t col = 0U; col < buf_img_width; col++) {
      line_buf[slot][col] = (RGBTRIPLE){0U, 0U, 0U};
   }
}

int line_buf_put_pixels(uint8_t slot, uint32_t x_start, RGBTRIPLE rgb,
                        uint32_t count) {
   /* Prüfen, ob Slot und Pixelbereich innerhalb des Puffers liegen */
   CHECK_OR_RETURN((uint32_t)slot < MAX_SCALE_FACTOR && slot < buf_box_size,
                   LINE_BUF_ERR);
   CHECK_OR_RETURN(x_start <= buf_img_width &&
                       count <= (buf_img_width - x_start),
                   LINE_BUF_ERR);

   for (uint32_t i = 0U; i < count; i++) {
      line_buf[slot][x_start + i] = rgb;
   }
   return EOK;
}

RGBTRIPLE line_buf_calc_box_avg(uint32_t x_start, uint8_t valid_rows) {
   RGBTRIPLE result = {0U, 0U, 0U};

   if ((x_start >= buf_img_width) || (valid_rows == 0U)) {
      return result;
   }

   uint8_t clamped_rows = valid_rows;
   if (clamped_rows > (uint8_t)MAX_SCALE_FACTOR) {
      clamped_rows = (uint8_t)MAX_SCALE_FACTOR;
   }

   /* Rechte Grenze der Kachel am Bildrand begrenzen */
   uint32_t x_end = x_start + (uint32_t)buf_box_size;
   if (x_end > buf_img_width) {
      x_end = buf_img_width;
   }

   /* Tatsächliche Pixelanzahl bestimmen (wichtig für Rand- und Eckkacheln) */
   uint32_t actual_width = x_end - x_start;
   uint32_t pixel_count = actual_width * (uint32_t)clamped_rows;

   uint32_t sum_red = 0U;
   uint32_t sum_green = 0U;
   uint32_t sum_blue = 0U;

   /* Rot-, Grün- und Blauwerte der Kachel getrennt aufsummieren */
   for (uint8_t row = 0U; row < clamped_rows; row++) {
      for (uint32_t col = x_start; col < x_end; col++) {
         sum_red += (uint32_t)line_buf[row][col].rgbtRed;
         sum_green += (uint32_t)line_buf[row][col].rgbtGreen;
         sum_blue += (uint32_t)line_buf[row][col].rgbtBlue;
      }
   }

   /* Mittelwert pro Farbkanal berechnen */
   if (pixel_count > 0U) {
      result.rgbtRed = (uint8_t)(sum_red / pixel_count);
      result.rgbtGreen = (uint8_t)(sum_green / pixel_count);
      result.rgbtBlue = (uint8_t)(sum_blue / pixel_count);
   }

   return result;
}
