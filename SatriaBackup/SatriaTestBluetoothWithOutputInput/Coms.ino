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
}