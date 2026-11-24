/**
 ******************************************************************************
 * @file    compressedBMP.c
 * @brief   Dekodiert RLE8-komprimierte 8-Bit-BMP-Bilder (Encoded Mode,
 *          Absolute Mode inkl. 16-Bit-Padding sowie EOL, EOB und Delta).
 ******************************************************************************
 */

#include "compressedBMP.h"
#include "BMP_types.h"
#include "colorTable.h"
#include "errorhandler.h"
#include "input.h"
#include "lineHandling.h"
#include <stdint.h>
#include <stdio.h>

/* Konstanten für RLE8-Steuerbytes */
#define BITS_PER_PIXEL_8 8U
#define RLE_ESCAPE_PREFIX 0U
#define RLE_ESC_EOL 0U
#define RLE_ESC_EOB 1U
#define RLE_ESC_DELTA 2U

int decode_compressed_bmp(const BITMAPINFOHEADER *info_hdr) {
   CHECK_OR_RETURN((info_hdr != NULL) &&
                       (info_hdr->biBitCount == BITS_PER_PIXEL_8),
                   UNSUPPORTED_FORMAT_ERR);

   for (;;) {
      /* Zwei Steuerbytes lesen und zuerst auf Dateiende (EOF) prüfen */
      int raw_first = nxt_chr();
      CHECK_OR_RETURN(raw_first != EOF, READ_ERR);
      int raw_second = nxt_chr();
      CHECK_OR_RETURN(raw_second != EOF, READ_ERR);

      uint8_t byte1 = (uint8_t)raw_first;
      uint8_t byte2 = (uint8_t)raw_second;

      if (byte1 > RLE_ESCAPE_PREFIX) {
         /* Encoded Mode: byte1 Pixel mit dem Palettenindex byte2 */
         RGBTRIPLE rgb = {0U, 0U, 0U};
         RETURN_ON_ERROR(get_color_from_table(byte2, &rgb));
         RETURN_ON_ERROR(put_pixel_run(rgb, (uint32_t)byte1));
      } else {
         /* Escape-Sequenzen (byte1 == 0) auswerten */
         switch (byte2) {
         case RLE_ESC_EOL:
            /* Zeilenende (00 00) */
            RETURN_ON_ERROR(finish_current_line());
            break;

         case RLE_ESC_EOB:
            /* Bildende (00 01): offene Zeile bzw. Rest-Streifen ausgeben */
            RETURN_ON_ERROR(flush_remaining_lines());
            return EOK;

         case RLE_ESC_DELTA: {
            /* Delta-Sprung (00 02 dx dy): Position um (dx, dy) verschieben */
            int raw_dx = nxt_chr();
            CHECK_OR_RETURN(raw_dx != EOF, READ_ERR);
            int raw_dy = nxt_chr();
            CHECK_OR_RETURN(raw_dy != EOF, READ_ERR);
            RETURN_ON_ERROR(apply_delta((uint8_t)raw_dx, (uint8_t)raw_dy));
            break;
         }

         default: {
            /* Absolute Mode (byte2 >= 3): byte2 Einzelpixel direkt einlesen */
            for (uint32_t i = 0U; i < (uint32_t)byte2; i++) {
               int raw_idx = nxt_chr();
               CHECK_OR_RETURN(raw_idx != EOF, READ_ERR);
               RGBTRIPLE rgb = {0U, 0U, 0U};
               RETURN_ON_ERROR(get_color_from_table((uint8_t)raw_idx, &rgb));
               RETURN_ON_ERROR(put_pixel_run(rgb, 1U));
            }
            /* Bei ungerader Pixelanzahl 1 Füllbyte (Word-Grenze) überlesen */
            if (((uint32_t)byte2 & 1U) != 0U) {
               int pad_raw = nxt_chr();
               CHECK_OR_RETURN(pad_raw != EOF, READ_ERR);
            }
            break;
         }
         }
      }
   }
}
