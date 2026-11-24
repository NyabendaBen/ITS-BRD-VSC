/**
 ******************************************************************************
 * @file    bmpApp.h
 * @brief   Ablaufsteuerung zum Laden, Prüfen und Anzeigen eines BMP-Bildes.
 ******************************************************************************
 */

#ifndef BMPAPP_H
#define BMPAPP_H

/**
 * @brief  Öffnet die nächste BMP-Datei, prüft Header, Größe und Offset,
 *         lädt bei Bedarf die Farbpalette und startet den passenden Decoder.
 * @retval EOK bei Erfolg, sonst ein Fehlercode aus errorhandler.h
 */
int handle_picture(void);

#endif /* BMPAPP_H */
