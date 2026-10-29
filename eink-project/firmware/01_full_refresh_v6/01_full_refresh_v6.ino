/*
  TEST 01 — Full-screen image / fast full refresh (based on PASSED V6)
  STATUS: Hardware-validated 2026-10-29: both patterns appeared correctly.
  PURPOSE: Test two distinct full-screen images and observe BUSY timings.
  EXPECTED: Pattern 1 (four corner squares) -> 5 s -> Pattern 2 (bars/text).
  SERIAL: 115200 baud; watch _Update_Full and BUSY transition count.
  REQUIRED: GxEPD2 and Adafruit GFX Library, DESPI-C02 at 3.3 V.
  PIN MAP: BUSY=5, RST=6, DC=7, CS=10, MOSI=11, SCK=12.
  IMPORTANT: N16R8 describes memory, not physical header positions.

  Open this .ino in Arduino IDE; verify, upload, and inspect Serial Monitor.
  Keep power OFF while changing wires or the flat-flex cable.
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

volatile uint32_t busyTransitions = 0;

void IRAM_ATTR busyChanged() {
  busyTransitions++;
}

void drawPattern(int pattern) {
  display.setFullWindow();
  unsigned long started = millis();

  Serial.print("Drawing pattern ");
  Serial.println(pattern);

  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);

    if (pattern == 1) {
      // Four corner blocks + a border and title.
      display.fillRect(10, 10, 200, 200, GxEPD_BLACK);
      display.fillRect(590, 10, 200, 200, GxEPD_BLACK);
      display.fillRect(10, 270, 200, 200, GxEPD_BLACK);
      display.fillRect(590, 270, 200, 200, GxEPD_BLACK);
      display.drawRect(0, 0, 800, 480, GxEPD_BLACK);

      display.setTextSize(3);
      display.setCursor(310, 220);
      display.print("GDEY075T7");
      display.setTextSize(2);
      display.setCursor(340, 270);
      display.print("PATTERN 1");
    } else {
      // Alternating vertical bars, then a black footer.
      for (int x = 0; x < 800; x += 100) {
        if ((x / 100) % 2 == 0) {
          display.fillRect(x, 0, 100, 130, GxEPD_BLACK);
        }
      }
      display.setTextSize(4);
      display.setCursor(275, 240);
      display.print("ESP32-S3");
      display.setTextSize(3);
      display.setCursor(310, 295);
      display.print("PATTERN 2");
      display.fillRect(0, 350, 800, 130, GxEPD_BLACK);
      display.setTextColor(GxEPD_WHITE);
      display.setTextSize(3);
      display.setCursor(220, 420);
      display.print("BOTTOM BLACK BAND");
    }
  } while (display.nextPage());

  Serial.print("Pattern refresh duration: ");
  Serial.print(millis() - started);
  Serial.println(" ms");
  Serial.print("BUSY transitions so far: ");
  Serial.println((uint32_t)busyTransitions);
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("=== DISPLAY TEST V6 ===");
  Serial.println("BUSY=5 RST=6 DC=7 CS=10 MOSI=11 SCK=12");

  pinMode(EPD_BUSY, INPUT);
  attachInterrupt(digitalPinToInterrupt(EPD_BUSY), busyChanged, CHANGE);
  SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);
  display.epd2.selectSPI(SPI, SPISettings(2000000, MSBFIRST, SPI_MODE0));
  display.init(115200);
  display.setRotation(0);

  Serial.print("Display width: ");
  Serial.println(display.width());
  Serial.print("Display height: ");
  Serial.println(display.height());

  drawPattern(1);
  Serial.println("Waiting 5 seconds...");
  delay(5000);
  drawPattern(2);

  Serial.print("Total BUSY transitions: ");
  Serial.println((uint32_t)busyTransitions);
  display.hibernate();
  Serial.println("=== TEST V6 COMPLETE ===");
}

void loop() {
  // One test sequence per reset. E-Ink holds the final image without refresh.
}
