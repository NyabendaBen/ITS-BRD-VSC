/**
 ******************************************************************************
 * @file    compressedBMP.h
 * @brief   Decoder für RLE8-komprimierte 8-Bit-BMP-Bilder (BI_RLE8).
 ******************************************************************************
 */

#ifndef COMPRESSEDBMP_H
#define COMPRESSEDBMP_H

#include "BMP_types.h"

/**
 * @brief  Dekodiert einen BI_RLE8-Datenstrom (Encoded Mode, Absolute Mode,
 *         EOL, EOB und Delta) und übergibt die Pixel an lineHandling.
 * @param  info_hdr  Geprüfter BITMAPINFOHEADER des aktuellen Bildes
 * @retval EOK bei Erfolg, sonst READ_ERR, FORMAT_ERR, INDEX_ERR,
 *         LINE_BUF_ERR oder UNSUPPORTED_FORMAT_ERR
 */
int decode_compressed_bmp(const BITMAPINFOHEADER *info_hdr);

#endif /* COMPRESSEDBMP_H */
