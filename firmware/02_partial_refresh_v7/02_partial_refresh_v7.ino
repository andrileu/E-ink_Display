/*
  TEST 02 — Center-window partial refresh (based on PASSED V7)
  STATUS: Hardware-validated 2026-10-29; no ghosting observed.
  PURPOSE: Change only a 320x136 window; retain static header and footer.
  EXPECTED: COUNT 0 -> 1 -> 2 -> 3 -> 4, then a final full refresh.
  SERIAL: 115200 baud; _Update_Part should be much faster than _Update_Full.
  REQUIRED: GxEPD2 + Adafruit GFX, known working hardware wiring.
  PIN MAP: BUSY=5, RST=6, DC=7, CS=10, MOSI=11, SCK=12.
  NOTE: Does not continuously refresh; repeated partials can eventually ghost.
*/
#include <Arduino.h>
#include <SPI.h>
#include <GxEPD2_BW.h>

#define EPD_CS    10
#define EPD_DC     7
#define EPD_RST    6
#define EPD_BUSY   5
#define EPD_SCK   12
#define EPD_MOSI  11

GxEPD2_BW<
  GxEPD2_750_GDEY075T7,
  GxEPD2_750_GDEY075T7::HEIGHT
> display(GxEPD2_750_GDEY075T7(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

// Byte-aligned update window. The permanent border is OUTSIDE this window.
const int PX = 240;
const int PY = 152;
const int PW = 320;
const int PH = 136;

volatile uint32_t busyTransitions = 0;

void IRAM_ATTR busyChanged() {
  busyTransitions++;
}

void drawStaticLayout() {
  display.fillScreen(GxEPD_WHITE);
  display.fillRect(0, 0, 800, 70, GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);
  display.setTextSize(3);
  display.setCursor(170, 20);
  display.print("PARTIAL UPDATE V7");

  display.fillRect(20, 130, 90, 90, GxEPD_BLACK);
  display.fillRect(690, 130, 90, 90, GxEPD_BLACK);
  display.drawRect(220, 130, 360, 180, GxEPD_BLACK);

  display.setTextColor(GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(150, 395);
  display.print("STATIC AREA - SHOULD NOT CHANGE");
}

void drawCounter(int value) {
  // Clear old digits before writing new ones (critical for partial updates).
  display.fillRect(PX, PY, PW, PH, GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);

  display.setTextSize(2);
  display.setCursor(300, 168);
  display.print("WINDOW UPDATE");
  display.setTextSize(4);
  display.setCursor(272, 205);
  display.print("COUNT: ");
  display.print(value);
  display.setTextSize(2);
  display.setCursor(300, 260);
  display.print("PARTIAL REFRESH");
}

void fullRefresh(int value) {
  display.setFullWindow();
  unsigned long started = millis();
  display.firstPage();
  do {
    drawStaticLayout();
    drawCounter(value);
  } while (display.nextPage());

  Serial.print("Full refresh duration: ");
  Serial.print(millis() - started);
  Serial.println(" ms");
}

void partialRefresh(int value) {
  display.setPartialWindow(PX, PY, PW, PH);
  unsigned long started = millis();
  display.firstPage();
  do {
    drawCounter(value);
  } while (display.nextPage());

  Serial.print("Partial refresh ");
  Serial.print(value);
  Serial.print(" duration: ");
  Serial.print(millis() - started);
  Serial.println(" ms");
  Serial.print("BUSY transitions: ");
  Serial.println((uint32_t)busyTransitions);
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("=== DISPLAY TEST V7 ===");

  pinMode(EPD_BUSY, INPUT);
  attachInterrupt(digitalPinToInterrupt(EPD_BUSY), busyChanged, CHANGE);
  SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);
  display.epd2.selectSPI(SPI, SPISettings(2000000, MSBFIRST, SPI_MODE0));
  display.init(115200);
  display.setRotation(0);

  Serial.print("Partial update support: ");
  Serial.println(display.epd2.hasPartialUpdate ? "YES" : "NO");
  Serial.print("Fast partial support: ");
  Serial.println(display.epd2.hasFastPartialUpdate ? "YES" : "NO");

  Serial.println("Drawing initial screen...");
  fullRefresh(0);
  delay(3000);

  // Four successive partial updates; watch for ghosting or unwanted flashing.
  for (int count = 1; count <= 4; count++) {
    Serial.print("Updating count to ");
    Serial.println(count);
    partialRefresh(count);
    delay(2000);
  }

  Serial.println("Final full refresh...");
  fullRefresh(4);
  display.hibernate();
  Serial.println("=== TEST V7 COMPLETE ===");
}

void loop() {
  // No repeating refreshes. Press Reset to rerun the experiment.
}
