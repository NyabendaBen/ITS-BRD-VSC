/**
 ******************************************************************************
 * @file    colorTable.c
 * @brief   Liest die 8-Bit-BMP-Farbpalette ein und schlägt Farbwerte nach.
 ******************************************************************************
 */

#include "colorTable.h"
#include "BMP_types.h"
#include "errorhandler.h"
#include "input.h"
#include <stddef.h>

/* Unterstützte Farbtiefen (8 Bit mit Palette, 24 Bit direkt) */
#define BITS_PER_PIXEL_8 8U
#define BITS_PER_PIXEL_24 24U

/* Statische Palette in .bss (1024 Bytes, damit der Stack frei bleibt) */
static RGBQUAD palette[MAX_COLOR_TABLE_SIZE];
static uint16_t num_colors = 0U;

int read_color_table(const BITMAPINFOHEADER *info_hdr, uint32_t *bytes_read) {
   CHECK_OR_RETURN((info_hdr != NULL) && (bytes_read != NULL), COL_TAB_ERR);

   /* Zustand vom vorherigen Bild zurücksetzen */
   *bytes_read = 0U;
   num_colors = 0U;

   /* 24-Bit-Bilder besitzen keine Farbpalette */
   if (info_hdr->biBitCount == BITS_PER_PIXEL_24) {
      return EOK;
   }

   /* Nur 8 Bit pro Pixel haben eine Palette */
   CHECK_OR_RETURN(info_hdr->biBitCount == BITS_PER_PIXEL_8, COL_TAB_ERR);

   /* Maximal 256 Einträge zulassen (Puffergrenze von palette[]) */
   CHECK_OR_RETURN(info_hdr->biClrUsed <= (uint32_t)MAX_COLOR_TABLE_SIZE,
                   COL_TAB_ERR);

   /* biClrUsed == 0 bedeutet laut BMP-Format alle 256 Farben */
   if (info_hdr->biClrUsed == 0U) {
      num_colors = (uint16_t)MAX_COLOR_TABLE_SIZE;
   } else {
      num_colors = (uint16_t)info_hdr->biClrUsed;
   }

   /* Paletteneinträge blockweise aus dem Eingabestrom lesen */
   int items = read_file((char *)palette, (unsigned int)sizeof(RGBQUAD),
                         (unsigned int)num_colors);
   CHECK_OR_RETURN(items == (int)num_colors, READ_ERR);

   *bytes_read = (uint32_t)num_colors * (uint32_t)sizeof(RGBQUAD);
   return EOK;
}

int get_color_from_table(uint8_t index, RGBTRIPLE *rgb) {
   CHECK_OR_RETURN(rgb != NULL, COL_TAB_ERR);

   /* Prüfen, ob der Index im geladenen Bereich liegt (z. B. bei biClrUsed = 3)
    */
   CHECK_OR_RETURN((uint16_t)index < num_colors, INDEX_ERR);

   /* RGBQUAD auf RGBTRIPLE kopieren (rgbReserved wird nicht benötigt) */
   rgb->rgbtBlue = palette[index].rgbBlue;
   rgb->rgbtGreen = palette[index].rgbGreen;
   rgb->rgbtRed = palette[index].rgbRed;

   return EOK;
}
