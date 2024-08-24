
#define ENABLE_MODULE_SERIAL_HARD
#define ENABLE_MODULE_SERIAL_SWAP
#define ENABLE_MODULE_DIGITAL_INPUT
#define ENABLE_MODULE_DIGITAL_OUTPUT

#include "Kinematrix.h"

HardSerial usbSerial;
SwapSerial bluetooth;

DigitalOut solenoid1(13, true);
DigitalOut solenoid2(26, true);
DigitalOut solenoid3(27, true);

void setup() {
  usbSerial.begin(&Serial, 115200);
  bluetooth.begin(&Serial2, 9600, SERIAL_8N1, 16, 17);  // Rx, Tx
}

void loop() {
  bluetooth.receiveString(onReceive);

  solenoid1.update();
}

void onReceive(const String& recvMsg) {
  Serial.print(F("| recvMsg: "));
  Serial.print(recvMsg);
  String dataHeader = usbSerial.getStrData(recvMsg, 0, "#");
  String dataValue = usbSerial.getStrData(recvMsg, 1, "#");
  if (isDigit(recvMsg[0]) || isDigit(recvMsg[1])) {
    // nums
  } else {
    dataHeader.toUpperCase();
    dataHeader.replace("\r", "");

    Serial.print(F("| dataHeader.length(): "));
    Serial.print(dataHeader.length());

    if (dataHeader == "BUKA1") solenoid1.on();
    if (dataHeader == "TUTUP1") solenoid1.off();

    if (dataHeader == "BUKA2") solenoid2.on();
    if (dataHeader == "TUTUP2") solenoid2.off();

    if (dataHeader == "BUKA3") solenoid3.on();
    if (dataHeader == "TUTUP3") solenoid3.off();
  }

  int state1 = solenoid1.getState();

  Serial.print(F("| state1: "));
  Serial.print(state1);
  Serial.println();

  int state2 = solenoid2.getState();

  Serial.print(F("| state2: "));
  Serial.print(state2);
  Serial.println();

  int state3 = solenoid3.getState();

  Serial.print(F("| state3: "));
  Serial.print(state3);
  Serial.println();
}