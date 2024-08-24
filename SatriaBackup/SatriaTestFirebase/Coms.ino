void onReceive(const String& recvMsg) {
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
          if (dataHeader == "BUKA" + String(i + 1)) solenoid[i].on();    // BUKA1
          if (dataHeader == "TUTUP" + String(i + 1)) solenoid[i].off();  // TUTUP1
        }
      }
      Serial.print(F("| recvMsg: "));
      Serial.println(recvMsg);
    }
  }
}