/**
 ******************************************************************************
 * @file    main.c
 * @brief   Hauptprogramm für GSP Aufgabe 3 (BITMAP-Bildanzeige auf dem
 *          ITS-Board). Initialisiert Hardware sowie USB/UART und zeigt in der
 *          Endlosschleife jeweils das nächste BMP-Bild auf dem LCD an.
 ******************************************************************************
 */

#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "bmpApp.h"
#include "error.h"
#include "fontsFLASH.h"
#include "init.h"
#include "input.h"
#include "usb_clock_config.h"
#include <timer.h>

int main(void) {
   initITSboard();      /* Board-Hardware initialisieren */
   switch_to_168_MHz(); /* 168 MHz für den 48-MHz-USB-Takt einstellen */
   initTimer();
   GUI_init(DEFAULT_BRIGHTNESS); /* LCD-Display und Touch initialisieren */
   TP_Init(false);
   if (!checkVersionFlashFonts()) {
      Error_Handler();
   }

   /* Übertragungsweg wählen: S7 = USB-CDC, S6 = UART-via-USB */
   transport_t transport = ask_transport();
   init_input(transport); /* Handshake (H/R/G) mit der Python-GUI */

   /* Hinweis anzeigen und auf S5 warten, bevor das erste Bild geladen wird */
   show_s5_hint();
   wait_for_s_i(BUTTON_S5);

   while (1) {
      /* Display vor der Header-Ausgabe bzw. einer Fehlermeldung leeren */
      GUI_clear(WHITE);

      /* Nächstes Bild einlesen, prüfen und anzeigen */
      (void)handle_picture();

      /* Bild oder Fehlermeldung anzeigen, bis S5 gedrückt wird */
      wait_for_s_i(BUTTON_S5);
   }
}
// EOF
