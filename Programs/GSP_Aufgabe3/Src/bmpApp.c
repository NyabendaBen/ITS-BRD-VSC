/**
 ******************************************************************************
 * @file    bmpApp.c
 * @brief   Steuert den kompletten Ablauf für ein BMP-Bild: Header lesen,
 *          Bildgröße und Offset prüfen, Farbpalette laden, Füllbytes
 *          überlesen, Zeilenverarbeitung starten und Decoder auswählen.
 ******************************************************************************
 */

#include "bmpApp.h"
#include "BMP_types.h"
#include "bmp_config.h"
#include "colorTable.h"
#include "compressedBMP.h"
#include "errorhandler.h"
#include "fileHeader.h"
#include "headers.h"
#include "input.h"
#include "lineHandling.h"
#include "uncompressedBMP.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BITS_PER_PIXEL_8 8U
#define HEADER_BYTES                                                           \
   ((uint32_t)sizeof(BITMAPFILEHEADER) + (uint32_t)sizeof(BITMAPINFOHEADER))

int handle_picture(void) {
   /* 1. Datei öffnen, Header prüfen & anzeigen, auf S5 warten, LCD leeren */
   RETURN_ON_ERROR(input_file_header());

   /* 2. Geprüfte Header-Strukturen holen (Stream steht bei Byte 54) */
   BITMAPFILEHEADER file_hdr;
   BITMAPINFOHEADER info_hdr;
   RETURN_ON_ERROR(get_file_hdr(&file_hdr));
   RETURN_ON_ERROR(get_info_hdr(&info_hdr));

   /* 3. Maximale Bildgröße prüfen (maximal 2400 x 1600 für Faktor 1/5) */
   CHECK_OR_RETURN((info_hdr.biWidth >= 1) &&
                       ((uint32_t)info_hdr.biWidth <= MAX_IMG_WIDTH),
                   IMG_SIZE_ERR);
   CHECK_OR_RETURN((info_hdr.biHeight >= 1) &&
                       ((uint32_t)info_hdr.biHeight <= MAX_IMG_HEIGHT),
                   IMG_SIZE_ERR);

   /* 4. Prüfen, ob bfOffBits groß genug für Header + Farbpalette ist */
   uint32_t expected_pal_bytes = 0U;
   int off_err = HEADER_ERR;
   if (info_hdr.biBitCount == BITS_PER_PIXEL_8) {
      uint32_t expected_colors = (info_hdr.biClrUsed == 0U)
                                     ? (uint32_t)MAX_COLOR_TABLE_SIZE
                                     : info_hdr.biClrUsed;
      expected_pal_bytes = expected_colors * (uint32_t)sizeof(RGBQUAD);
      off_err = COL_TAB_ERR;
   }
   uint32_t min_off_bits = HEADER_BYTES + expected_pal_bytes;
   CHECK_OR_RETURN(file_hdr.bfOffBits >= min_off_bits, off_err);

   /* 5. Farbpalette laden und eventuelle Füllbytes bis bfOffBits überlesen */
   uint32_t pal_bytes = 0U;
   RETURN_ON_ERROR(read_color_table(&info_hdr, &pal_bytes));

   uint32_t stream_pos = HEADER_BYTES + pal_bytes;
   CHECK_OR_RETURN(file_hdr.bfOffBits >= stream_pos, off_err);

   uint32_t gap_bytes = file_hdr.bfOffBits - stream_pos;
   for (uint32_t gap_idx = 0U; gap_idx < gap_bytes; gap_idx++) {
      int pad_byte = nxt_chr();
      CHECK_OR_RETURN(pad_byte != EOF, READ_ERR);
   }

   /* 6. Zeilenverarbeitung und Skalierungsfaktor vorbereiten */
   RETURN_ON_ERROR(init_line_handling((uint32_t)info_hdr.biWidth,
                                      (uint32_t)info_hdr.biHeight));

   /* 7. Passenden Decoder je nach Komprimierung starten */
   if (info_hdr.biCompression == (DWORD)BI_RLE8) {
      RETURN_ON_ERROR(decode_compressed_bmp(&info_hdr));
      return EOK;
   }
   if (info_hdr.biCompression == (DWORD)BI_RGB) {
      RETURN_ON_ERROR(decode_uncompressed_bmp(&info_hdr));
      return EOK;
   }

   return CHECK(false, UNSUPPORTED_FORMAT_ERR);
}
