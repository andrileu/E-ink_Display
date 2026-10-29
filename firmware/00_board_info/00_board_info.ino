/*
  TEST 00 — ESP32-S3 board and serial diagnostics
  STATUS: Utility; safe to run with or without the e-paper connected.
  PURPOSE: Report chip, flash and PSRAM information; verify Serial output.
  HARDWARE: ESP32-S3 N16R8, connected by USB data cable.
  OUTPUT: Prints system information once, then a heartbeat every 3 seconds.
  SERIAL: 115200 baud.
  DOES NOT draw on the E-Ink display.

  Open THIS .ino file in Arduino IDE and use Tools > Board > ESP32S3 Dev Module.
  Actual USB and memory selections: docs/arduino-ide.md.
*/
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(2000);  // Allow the USB serial monitor time to connect.

  Serial.println();
  Serial.println("=== ESP32-S3 BOARD INFO ===");
  Serial.print("Chip: ");
  Serial.println(ESP.getChipModel());
  Serial.printf("Flash: %lu bytes\n", (unsigned long)ESP.getFlashChipSize());
  Serial.printf("PSRAM available: %s\n", psramFound() ? "YES" : "NO");
  Serial.printf("PSRAM: %lu bytes\n", (unsigned long)ESP.getPsramSize());
  Serial.println("Expected for N16R8: about 16 MB flash and 8 MB PSRAM.");
}

void loop() {
  Serial.println("ESP32-S3 IS ALIVE");
  delay(3000);
}
