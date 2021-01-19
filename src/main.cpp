#include <Arduino.h>

#include "SerialDataExporter.h"

int bufferSizes[] = {255, 5};
SerialDataExporter exporter = SerialDataExporter(Serial, bufferSizes);

void setup() {
  Serial.begin(9600);
  delay(250);
  // put your setup code here, to run once:
}

int counter = 0;
int counter2 = 1;
void loop() {
  exporter.add("x", counter);
  exporter.add("y", counter2);
  exporter.exportToSerial();

  counter++;
  counter2++;
  delay(500);
}
