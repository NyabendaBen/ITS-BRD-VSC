/**
 ******************************************************************************
 * @file    uncompressedBMP.c
 * @brief   Dekodiert unkomprimierte 8-Bit- und 24-Bit-BMP-Dateien (BI_RGB)
 *          zeilenweise inklusive 4-Byte-Zeilen-Padding.
 ******************************************************************************
 */

#include "uncompressedBMP.h"
#include "BMP_types.h"
#include "colorTable.h"
#include "errorhandler.h"
#include "input.h"
#include "lineHandling.h"
#include <stdint.h>
#include <stdio.h>

/* Konstanten für Farbtiefe und 4-Byte-Zeilenausrichtung */
#define BITS_PER_PIXEL_8 8U
#define BITS_PER_PIXEL_24 24U
#define BITS_PER_BYTE 8U
#define DWORD_ROUND_ADD 31U
#define BITS_PER_DWORD 32U
#define BYTES_PER_DWORD 4U

/* Fasst die Zeilendaten (Breite, Bittiefe, Padding) für Hilfsfunktionen
 * zusammen */
typedef struct {
   uint32_t width;
   uint32_t bpp;
   uint32_t padding_bytes;
} RowFormat;

/* Liest ein Pixel (8-Bit-Index oder 24-Bit-RGBTRIPLE) und übergibt es weiter */
static int decode_uncompressed_pixel(uint32_t bpp) {
   RGBTRIPLE rgb = {0U, 0U, 0U};

   if (bpp == BITS_PER_PIXEL_8) {
      /* 8 Bit: Index lesen und Farbe in der Palette nachschlagen */
      int raw_idx = nxt_chr();
      CHECK_OR_RETURN(raw_idx != EOF, READ_ERR);
      RETURN_ON_ERROR(get_color_from_table((uint8_t)raw_idx, &rgb));
   } else {
      /* 24 Bit: 3 Farbbytes (Blue, Green, Red) direkt lesen */
      int items = read_file((char *)&rgb, (unsigned int)sizeof(RGBTRIPLE), 1U);
      CHECK_OR_RETURN(items == 1, READ_ERR);
   }

   return put_pixel_run(rgb, 1U);
}

/* Überliest die 0..3 Füllbytes am Ende einer unkomprimierten Bildzeile */
static int skip_row_padding(uint32_t padding_bytes) {
   for (uint32_t pad_idx = 0U; pad_idx < padding_bytes; pad_idx++) {
      int pad_raw = nxt_chr();
      CHECK_OR_RETURN(pad_raw != EOF, READ_ERR);
   }
   return EOK;
}

/* Dekodiert eine komplette Zeile inklusive Zeilen-Padding */
static int decode_single_row(const RowFormat *row_fmt) {
   for (uint32_t col = 0U; col < row_fmt->width; col++) {
      RETURN_ON_ERROR(decode_uncompressed_pixel(row_fmt->bpp));
   }

   RETURN_ON_ERROR(skip_row_padding(row_fmt->padding_bytes));
   return finish_current_line();
}

int decode_uncompressed_bmp(const BITMAPINFOHEADER *info_hdr) {
   CHECK_OR_RETURN((info_hdr != NULL) && (info_hdr->biWidth >= 1) &&
                       (info_hdr->biHeight >= 1),
                   FORMAT_ERR);
   CHECK_OR_RETURN((info_hdr->biBitCount == BITS_PER_PIXEL_8) ||
                       (info_hdr->biBitCount == BITS_PER_PIXEL_24),
                   UNSUPPORTED_FORMAT_ERR);

   uint32_t width = (uint32_t)info_hdr->biWidth;
   uint32_t height = (uint32_t)info_hdr->biHeight;
   uint32_t bpp = (uint32_t)info_hdr->biBitCount;

   /* Zeilenlänge auf Vielfaches von 4 Bytes aufrunden (BMP-Spezifikation) */
   uint32_t row_stride_bytes =
       (((width * bpp) + DWORD_ROUND_ADD) / BITS_PER_DWORD) * BYTES_PER_DWORD;
   uint32_t payload_bytes = width * (bpp / BITS_PER_BYTE);

   RowFormat row_fmt = {width, bpp, row_stride_bytes - payload_bytes};

   /* Alle Zeilen von unten nach oben einlesen */
   for (uint32_t row = 0U; row < height; row++) {
      RETURN_ON_ERROR(decode_single_row(&row_fmt));
   }

   return flush_remaining_lines();
}
