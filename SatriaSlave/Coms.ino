void onReceive(const String& recvMsg) {
  Serial.print(F("| recvMsg: "));
  Serial.println(recvMsg);
  String dataHeader = usbSerial.getStrData(recvMsg, 0, "#");
  String dataValue = usbSerial.getStrData(recvMsg, 1, "#");
  if (isDigit(recvMsg[0]) || isDigit(recvMsg[1])) {
    // nums
  } else {
    dataHeader.toUpperCase();
    dataHeader.replace("\r", "");
    if (dataHeader == "R") ESP.restart();
    if (dataHeader == "T") camTriggerSerial = 1;
  }
}