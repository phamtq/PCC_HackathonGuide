#include <PxMatrix.h>
#include <avr/pgmspace.h>
#include "rainbowcat.h"

// ---------------- PIN DEFINITIONS ----------------
#define P_LAT 22
#define P_OE  16
#define P_A   2
#define P_B   3
#define P_C   4
#define P_D   5
#define P_E   6

// ---------------- DISPLAY ----------------
PxMATRIX display(64, 64, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

const uint8_t DRAW_TIME = 20;

// ---------------- SETUP ----------------
void setup() {
  display.begin(32);
  display.setBrightness(120);
  display.clearDisplay();

  const int IMG_W = 62;
  const int IMG_H = 63;

  int offsetX = (64 - IMG_W) / 2;
  int offsetY = (64 - IMG_H) / 2;

  for (int y = 0; y < IMG_H; y++) {
    for (int x = 0; x < IMG_W; x++) {

      int i = (y * IMG_W + x) * 2;

      uint8_t low  = pgm_read_byte(&RAINBOWCAT_map[i]);
      uint8_t high = pgm_read_byte(&RAINBOWCAT_map[i + 1]);

      uint16_t color = low | (high << 8);

      display.drawPixel(x + offsetX, y + offsetY, color);
    }
  }
}

// ---------------- LOOP ----------------
void loop() {
  display.display(DRAW_TIME);
}
