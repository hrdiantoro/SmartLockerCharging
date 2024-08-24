
#define ENABLE_MODULE_SERIAL_HARD
#define ENABLE_MODULE_SERIAL_SWAP

#include "Kinematrix.h"

HardSerial usbSerial;
SwapSerial bluetooth;

void setup() {
  usbSerial.begin(&Serial, 115200);
  bluetooth.begin(&Serial2, 9600, SERIAL_8N1, 16, 17);  // Rx, Tx
}

void loop() {
  bluetooth.receiveString(onReceive);

  // bluetooth.receiveString([](const String& recvMsg) -> void {
  //   Serial.println(recvMsg);
  // });
}

void onReceive(const String& recvMsg) {
  Serial.println(recvMsg);
}