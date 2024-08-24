void wifiConTask() {
  disableLoopWDT();
  disableCore0WDT();
  disableCore1WDT();
  task.createTask(10000, [](void* pvParameter) {
    auth.apiKey = "AIzaSyAJXsdP44JetbAbqXwnD1F6agXFPk-gyyM";
    auth.databaseURL = "https://loker-apps-default-rtdb.firebaseio.com/";
    auth.projectID = "loker-apps";

    auth.user.email = "admin@gmail.com";
    auth.user.password = "admin123";

    // if (!wm.autoConnect("SatriaAP", ""))  // 192.168.4.1
    // Serial.println("| Failed to connect");
    firebase.connectToWiFi("TIMEOSPACE", "1234Saja");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    firebase.init(&auth);

    for (;;) {
      if (firebase.isConnect()) {

        uint32_t epoch = getTime();
        epoch += gmtOffset_sec;
        DateTimeNTP currentTime(epoch);
        var.nowDate = DateTime(currentTime.timestamp().c_str());

        static uint32_t getDataTimer = millis();
        if (millis() - getDataTimer >= 4000) {
          firebase.firestoreListDocument(
            "products", LOCKER_TOTAL, [](JsonVariant res, size_t pageSize) {
              // serializeJsonPretty(res, Serial);
              for (int i = 0; i < pageSize; i++) {
                var.locker[i].endDate = DateTime(res[i]["fields"]["endDate"]["stringValue"].as<String>().c_str());
                var.locker[i].name = res[i]["fields"]["nama_loker"]["stringValue"].as<String>();
                var.locker[i].openMsg = res[i]["fields"]["pesan_buka"]["stringValue"].as<String>();
                var.locker[i].closeMsg = res[i]["fields"]["pesan_tutup"]["stringValue"].as<String>();
                // var.locker[i].status = res[i]["fields"]["status"]["booleanValue"].as<bool>();
              }
            },
            FirebaseModule::resultStatusCallback);
          getDataTimer = millis();
        }
        var.firebaseReady = 1;
      }
      task.delay(20);
    }
  });

  task.createTask(10000, [](void* pvParameter) {
    initState();
    for (;;) {
      if (var.firebaseReady) {
        for (int i = 0; i < LOCKER_TOTAL; i++) {
          var.locker[i].status = var.nowDate < var.locker[i].endDate;
          if (!var.locker[i].status) {
            solenoid[i].off();
          } else {
            // var.magneticState[i] = magnetic[i].getState();
            var.magneticState[i] = solenoid[i].getState();
            if (var.magneticState[i] != var.magneticStateBefore[i]) {
              // if (!var.magneticState[i]) {
              if (var.magneticState[i]) {
                cam[i].on();
                cam[i].offDelay(2000);
              }
              var.magneticStateBefore[i] = var.magneticState[i];
            }
          }
        }

        if (var.debugLevel == DEBUG_TIME) {
          Serial.print("| n: ");
          Serial.print(var.nowDate.timestamp());
          Serial.print("| 1: ");
          Serial.print(var.locker[LOCKER_A].endDate.timestamp());
          Serial.print("| 2: ");
          Serial.print(var.locker[LOCKER_B].endDate.timestamp());
          Serial.print("| 3: ");
          Serial.print(var.locker[LOCKER_C].endDate.timestamp());
          Serial.println();
        }
      }
      task.delay(20);
    }
  });
}

void initState() {
  for (int i = 0; i < LOCKER_TOTAL; i++) {
    var.magneticState[i] = 0;
    var.magneticStateBefore[i] = 0;
  }
}