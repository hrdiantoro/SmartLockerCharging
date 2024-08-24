#include "Arduino.h"
#include "WiFi.h"
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
#include <LittleFS.h>
#include <FS.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include "configHeader.h"

#define ENABLE_MODULE_DIGITAL_INPUT
#define ENABLE_MODULE_DIGITAL_OUTPUT
#define ENABLE_MODULE_DATETIME_NTP

#define ENABLE_SENSOR_MODULE
#define ENABLE_SENSOR_MODULE_UTILITY
#define ENABLE_SENSOR_RTC

#include "Kinematrix.h"

const char* ssid = "TIMEOSPACE";
const char* password = "1234Saja";

bool takeNewPhoto = true;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;

DigitalIn camTrigger(16, INPUT_PULLDOWN);
DigitalOut camFlashLight(4);

void setup() {
  Serial.begin(115200);
  initWiFi();
  initLittleFS();
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  initCamera();
  configF.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  configF.token_status_callback = tokenStatusCallback;
  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (camTrigger.isPressed()) {
    camFlashLight.on();
    capturePhotoSaveLittleFS();
    camFlashLight.off();

    bool firebaseReady = false;
    while (!firebaseReady) {
      firebaseReady = Firebase.ready();
    }
    Serial.print("Uploading picture... ");
    if (Firebase.Storage.upload(
          &fbdo,                  /* Firebase data object */
          STORAGE_BUCKET_ID,      /* Firebase Storage bucket id */
          FILE_PHOTO_PATH,        /* path to local file */
          mem_storage_type_flash, /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */
          BUCKET_PHOTO,           /* path of remote file stored in the bucket */
          "image/jpeg",           /* mime type */
          fcsUploadCallback)) {
      Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
    } else {
      Serial.println(fbdo.errorReason());
    }
  }
  camTrigger.update();
  camFlashLight.update();
}