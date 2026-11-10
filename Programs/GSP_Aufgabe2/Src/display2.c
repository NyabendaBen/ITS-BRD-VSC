#include "display2.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include <stdio.h>

#define ANGLE_ROW 1U
#define SPEED_ROW 3U
#define VALUE_COL 18U
#define NUMBER_WIDTH 8U
#define TOTAL_CHARS 16U
#define BUFFER_SIZE (TOTAL_CHARS + 1U) // snprintf Buffer Größe

typedef enum {
   DISP_IDLE = 0,
   DISP_BUSY,
} DisplayState;

static DisplayState state = DISP_IDLE;
static char buffer[BUFFER_SIZE];
static uint8_t pos = 0;

// Display initialisieren und feste Beschriftung zeichnen
void display_init(void) {
   GUI_init(DEFAULT_BRIGHTNESS);
   TP_Init(false);

   lcdGotoXY(0, ANGLE_ROW);
   lcdPrintS("Winkel          :           Grad");

   lcdGotoXY(0, SPEED_ROW);
   lcdPrintS("Geschwindigkeit :           Grad/s");

   state = DISP_IDLE;
   pos = 0;
}

// Neue Werte in den Puffer formatieren
void display_request(const Measurement *meas) {
   if (meas == NULL) {
      return;
   }

   snprintf(&buffer[0], NUMBER_WIDTH + 1U, "%8.1f", meas->angle_deg);
   snprintf(&buffer[NUMBER_WIDTH], NUMBER_WIDTH + 1U, "%8.1f",
            meas->speed_deg_per_s);

   pos = 0;
   state = DISP_BUSY;
}

// Pro Aufruf maximal 1 Zeichen ausgeben
void display_process(void) {
   if (state != DISP_BUSY) {
      return;
   }

   if (pos < NUMBER_WIDTH) {
      lcdGotoXY(VALUE_COL + pos, ANGLE_ROW);
   }
   if (pos >= NUMBER_WIDTH) {
      lcdGotoXY(VALUE_COL + (pos - NUMBER_WIDTH), SPEED_ROW);
   }

   lcdPrintC(buffer[pos]);
   pos++;

   if (pos >= TOTAL_CHARS) {
      state = DISP_IDLE;
   }
}

// Prüft, ob die Ausgabe noch läuft
bool display_is_busy(void) { return state == DISP_BUSY; }
