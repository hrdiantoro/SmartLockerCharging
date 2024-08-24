#include "Header.h"

void setup() {
  usbSerial.begin(&Serial, 115200);
  bluetooth.begin(&Serial2, 9600, SERIAL_8N1, 16, 17);  // Rx, Tx

  task.initialize(wifiConTask);
}

void loop() {
  debug();

  usbSerial.receiveString(onReceive);
  bluetooth.receiveString(onReceive);

  for (int i = 0; i < LOCKER_TOTAL; i++) {
    magnetic[i].update();
    solenoid[i].update();
    cam[i].update();
  }
}

void debug() {
  if (var.debugLevel == DEBUG_IO) {
    Serial.print("| s1: ");
    Serial.print(var.locker[LOCKER_A].status);
    Serial.print("| s2: ");
    Serial.print(var.locker[LOCKER_B].status);
    Serial.print("| s3: ");
    Serial.print(var.locker[LOCKER_C].status);

    Serial.print("| mg1: ");
    Serial.print(magnetic[LOCKER_A].getState());
    Serial.print("| mg2: ");
    Serial.print(magnetic[LOCKER_B].getState());
    Serial.print("| mg3: ");
    Serial.print(magnetic[LOCKER_C].getState());

    Serial.print("| sol1: ");
    Serial.print(solenoid[LOCKER_A].getState());
    Serial.print("| sol2: ");
    Serial.print(solenoid[LOCKER_B].getState());
    Serial.print("| sol3: ");
    Serial.print(solenoid[LOCKER_C].getState());

    Serial.print("| c1: ");
    Serial.print(cam[LOCKER_A].getState());
    Serial.print("| c2: ");
    Serial.print(cam[LOCKER_B].getState());
    Serial.print("| c3: ");
    Serial.print(cam[LOCKER_C].getState());
    Serial.println();
  }
}