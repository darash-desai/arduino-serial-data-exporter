#include <Arduino.h>
#include "SerialDataExporter.h"

int bufferSizes[] = {255, 5, 5};
SerialDataExporter exporter = SerialDataExporter(Serial, bufferSizes);

void setup() {
  Serial.begin(9600);
  delay(250);
  // put your setup code here, to run once:
}

int counter = 0;
int counter2 = 1;
double counter3 = 3.2987203082402;
void loop() {
  exporter.add("x", counter);
  exporter.add("y", counter2);
  exporter.add("z", counter3);
  exporter.exportJSON();

  counter++;
  counter2++;
  counter3++;
  delay(500);
}
