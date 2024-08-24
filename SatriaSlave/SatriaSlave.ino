#define ENABLE_MODULE_SERIAL_HARD
#define ENABLE_MODULE_DIGITAL_INPUT
#define ENABLE_MODULE_DIGITAL_OUTPUT
#define ENABLE_MODULE_DATETIME_NTP

#define ENABLE_SENSOR_MODULE
#define ENABLE_SENSOR_MODULE_UTILITY
#define ENABLE_SENSOR_RTC

#include "Kinematrix.h"
#include "configHeader.h"

#define SLAVE_ADDRESS 3

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;  // Offset for WIB (UTC+7)
const int daylightOffset_sec = 0;

const char* ssid = "TIMEOSPACE";
const char* password = "1234Saja";

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;

HardSerial usbSerial;

DigitalIn camTrigger(14, INPUT_PULLDOWN);
DigitalOut camFlashLight(4);
DigitalOut camLedBuiltin(33, true);

enum SystemEnum {
  SYSTEM_DISABLE,
  SYSTEM_ENABLE,
  SYSTEM_TESTING,
};

int camStateInitCamera = SYSTEM_ENABLE;
int camTriggerSerial = 0;

void setup() {
  usbSerial.begin(&Serial, 115200);

  disableLoopWDT();
  disableCore1WDT();
  disableCore0WDT();

  Serial.println("initWiFi();");
  initWiFi();

  Serial.println("initLittleFS();");
  initLittleFS();

  Serial.println("WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);");
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.println("initCamera();");
  initCamera();

  configF.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  configF.token_status_callback = tokenStatusCallback;
  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  if (camStateInitCamera == SYSTEM_ENABLE) {
    if (camTrigger.getStateRaw() == 1 || camTriggerSerial == 1) {
      captureAndSaveToFirebase();
      camTriggerSerial = 0;
    }
  } else if (camStateInitCamera == SYSTEM_TESTING) {
    captureAndSaveToFirebase();
    delay(20000);
  }

  usbSerial.receiveString(onReceive);
  camLedBuiltin.toggleAsync(500);

  camTrigger.update();
  camFlashLight.update();
  camLedBuiltin.update();
}

void captureAndSaveToFirebase() {
  camFlashLight.on();
  capturePhotoSaveLittleFS();
  camFlashLight.off();
  while (!Firebase.ready()) {
    camLedBuiltin.toggle();
    delay(50);
  }
  camFlashLight.off();
  Serial.print("Uploading picture... ");

  String epoch = String(getTime() + gmtOffset_sec);
  String address = String(SLAVE_ADDRESS);

  if (Firebase.Storage.upload(
        &fbdo,                                            /* Firebase data object */
        STORAGE_BUCKET_ID,                                /* Firebase Storage bucket id */
        FILE_PHOTO_PATH,                                  /* path to local file */
        mem_storage_type_flash,                           /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */
        "/Loker " + address + "/photo_" + epoch + ".jpg", /* path of remote file stored in the bucket */
        "image/jpeg",                                     /* mime type */
        fcsUploadCallback)) {
    Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
  } else {
    Serial.println(fbdo.errorReason());
  }
}