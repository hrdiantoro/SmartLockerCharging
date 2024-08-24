#include "Header.h"

void setup() {
  usbSerial.begin(&Serial, 115200);
  bluetooth.begin(&Serial2, 9600, SERIAL_8N1, 16, 17);  // Rx, Tx
}

void loop() {
  bluetooth.receiveString(onReceive);

  Serial.print("| magnetic1: ");
  Serial.print(magnetic1.getState());
  Serial.print("| magnetic2: ");
  Serial.print(magnetic2.getState());
  Serial.print("| magnetic3: ");
  Serial.print(magnetic3.getState());
  Serial.println();

  DigitalIn::updateAll(&magnetic1, &magnetic2, &magnetic3, DigitalIn::stop());
  DigitalOut::updateAll(&solenoid1, &solenoid2, &solenoid3, &cam1, &cam2, &cam3, DigitalOut::stop());
}