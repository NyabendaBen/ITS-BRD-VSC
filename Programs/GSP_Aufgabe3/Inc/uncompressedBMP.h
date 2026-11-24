/**
 ******************************************************************************
 * @file    uncompressedBMP.h
 * @brief   Decoder für unkomprimierte 8-Bit- und 24-Bit-BMP-Bilder (BI_RGB).
 ******************************************************************************
 */

#ifndef UNCOMPRESSEDBMP_H
#define UNCOMPRESSEDBMP_H

#include "BMP_types.h"

/**
 * @brief  Dekodiert ein unkomprimiertes BMP-Bild (8 Bit mit Palette oder
 *         24 Bit direkt) zeilenweise inkl. 4-Byte-Padding.
 * @param  info_hdr  Geprüfter BITMAPINFOHEADER des aktuellen Bildes
 * @retval EOK bei Erfolg, sonst READ_ERR, FORMAT_ERR, INDEX_ERR,
 *         LINE_BUF_ERR oder UNSUPPORTED_FORMAT_ERR
 */
int decode_uncompressed_bmp(const BITMAPINFOHEADER *info_hdr);

#endif /* UNCOMPRESSEDBMP_H */
