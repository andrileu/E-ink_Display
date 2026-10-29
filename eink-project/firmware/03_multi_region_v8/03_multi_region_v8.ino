/*
  TEST 03 — Independent dashboard update regions (based on V8)
  STATUS: EXPERIMENTAL — user said "worked approximately well" on 2026-10-29.
  PURPOSE: Update four different areas (number, text, bar, black/white flag).
  EXPECTED: 5 partial changes, then a full refresh to clean up artifacts.
  SERIAL: 115200 baud; log each partial window's timing.
  REQUIRED: GxEPD2 + Adafruit GFX.
  PIN MAP: BUSY=5, RST=6, DC=7, CS=10, MOSI=11, SCK=12.
  NOTE: Keep this separate from the V6/V7 PASSED reference tests.
  TODO: Inspect alignment/text rendering and revise only after re-testing.
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

// X and width must align to multiples of 8 for monochrome update windows.
const int WX[] = {64, 472, 136, 384};
const int WY[] = {152, 152, 328, 416};
const int WW[] = {160, 192, 528, 32};
const int WH[] = {64, 64, 32, 32};

volatile uint32_t busyTransitions = 0;
void IRAM_ATTR busyChanged() { busyTransitions++; }

void drawStatic() {
  display.fillScreen(GxEPD_WHITE);
  display.fillRect(0, 0, 800, 64, GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);
  display.setTextSize(3);
  display.setCursor(170, 20);
  display.print("E-INK DASHBOARD V8");

  display.setTextColor(GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(64, 112);
  display.print("VALUE");
  display.drawRect(56, 144, 176, 80, GxEPD_BLACK);

  display.setCursor(472, 112);
  display.print("STATUS");
  display.drawRect(464, 144, 208, 80, GxEPD_BLACK);

  display.setCursor(128, 276);
  display.print("PROGRESS");
  display.drawRect(128, 320, 544, 48, GxEPD_BLACK);

  display.setCursor(310, 420);
  display.print("FLAG");
  display.drawRect(376, 408, 48, 48, GxEPD_BLACK);

  display.setTextSize(2);
  display.setCursor(200, 456);
  display.print("FOUR UPDATE REGIONS");
}

void drawRegion(int region, int value) {
  // Remove previous pixels before drawing this region's replacement.
  display.fillRect(WX[region], WY[region], WW[region], WH[region], GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);

  switch (region) {
    case 0:
      display.setTextSize(4);
      display.setCursor(96, 172);
      display.print(value);
      display.print("C");
      break;
    case 1:
      display.setTextSize(3);
      display.setCursor(488, 172);
      display.print(value == 0 ? "IDLE" : "ACTIVE");
      break;
    case 2:
      // Use integer arithmetic, fill the interior of the progress-bar border.
      display.fillRect(WX[2], WY[2], WW[2] * value / 100, WH[2], GxEPD_BLACK);
      break;
    case 3:
      if (value == 1) {
        display.fillRect(WX[3], WY[3], WW[3], WH[3], GxEPD_BLACK);
      }
      break;
  }
}

void drawDashboard(bool finalState) {
  drawStatic();
  drawRegion(0, finalState ? 22 : 21);
  drawRegion(1, finalState ? 1 : 0);
  drawRegion(2, finalState ? 70 : 20);
  drawRegion(3, 0);  // After the test, flag returns to white.
}

void fullRefresh(bool finalState) {
  display.setFullWindow();
  uint32_t started = millis();
  display.firstPage();
  do {
    drawDashboard(finalState);
  } while (display.nextPage());
  Serial.printf("FULL refresh: %lu ms\n", (unsigned long)(millis() - started));
}

void updatePartial(int region, int value, const char* label) {
  Serial.print("Updating: ");
  Serial.println(label);
  display.setPartialWindow(WX[region], WY[region], WW[region], WH[region]);
  uint32_t started = millis();
  uint32_t before = busyTransitions;

  display.firstPage();
  do {
    drawRegion(region, value);
  } while (display.nextPage());

  Serial.printf("Window: %d x %d; duration: %lu ms; BUSY transitions: %lu\n",
                WW[region], WH[region], (unsigned long)(millis() - started),
                (unsigned long)(busyTransitions - before));
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("=== DISPLAY TEST V8 (EXPERIMENTAL) ===");

  pinMode(EPD_BUSY, INPUT);
  attachInterrupt(digitalPinToInterrupt(EPD_BUSY), busyChanged, CHANGE);
  SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);
  display.epd2.selectSPI(SPI, SPISettings(2000000, MSBFIRST, SPI_MODE0));
  display.init(115200);
  display.setRotation(0);

  Serial.println("Initial full refresh...");
  fullRefresh(false);
  delay(2000);

  updatePartial(0, 22, "VALUE 21C -> 22C");
  delay(1200);
  updatePartial(1, 1, "STATUS IDLE -> ACTIVE");
  delay(1200);
  updatePartial(2, 70, "PROGRESS 20 -> 70");
  delay(1200);
  updatePartial(3, 1, "FLAG WHITE -> BLACK");
  delay(1200);
  updatePartial(3, 0, "FLAG BLACK -> WHITE");
  delay(1200);

  Serial.println("Final full refresh...");
  fullRefresh(true);
  display.hibernate();
  Serial.println("=== TEST V8 COMPLETE ===");
}

void loop() {
  // Test is intentionally one-shot to avoid unnecessary panel wear.
}
