void onReceive(const String& recvMsg) {
  uint32_t startTime, endTime, delayTime;
  startTime = micros();

  // Serial.print(F("| recvMsg: "));
  // Serial.println(recvMsg);
  String dataHeader = usbSerial.getStrData(recvMsg, 0, "#");
  String dataValue = usbSerial.getStrData(recvMsg, 1, "#");
  if (isDigit(recvMsg[0]) || isDigit(recvMsg[1])) {
    // nums
  } else {
    dataHeader.toUpperCase();
    dataHeader.replace("\r", "");

    if (dataHeader == "R") ESP.restart();
    if (dataHeader == "D") var.debugLevel = dataValue.toInt();

    if (var.firebaseReady) {
      for (int i = 0; i < LOCKER_TOTAL; i++) {
        if (var.locker[i].status) {
          endTime = micros();
          delayTime = endTime - startTime;
          float delayTimeF = (float)delayTime;

          if (dataHeader == "BUKA" + String(i + 1)) {  // BUKA1
            solenoid[i].on();
            Serial.print("| BUKA " + String(i + 1) + ": ");
            Serial.print("| delayTimeF: ");
            Serial.print(delayTimeF);
            Serial.print(" microSeconds");
            Serial.println();
          }
          if (dataHeader == "TUTUP" + String(i + 1)) {  // TUTUP1
            solenoid[i].off();
            Serial.print("| TUTUP " + String(i + 1) + ": ");
            Serial.print("| delayTimeF: ");
            Serial.print(delayTimeF);
            Serial.print(" microSeconds");
            Serial.println();
          }
        }
      }
      // Serial.print(F("| recvMsg: "));
      // Serial.println(recvMsg);
    }
  }
}