#define ENABLE_MODULE_SERIAL_HARD
#define ENABLE_MODULE_SERIAL_SWAP
#define ENABLE_MODULE_DIGITAL_INPUT
#define ENABLE_MODULE_DIGITAL_OUTPUT
#define ENABLE_MODULE_FIREBASE_HANDLER
#define ENABLE_MODULE_TASK_HANDLER
#define ENABLE_MODULE_DATETIME_NTP

#define ENABLE_SENSOR_MODULE
#define ENABLE_SENSOR_MODULE_UTILITY
#define ENABLE_SENSOR_RTC

#include "Kinematrix.h"

////////// Utility //////////
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;  // Offset for WIB (UTC+7)
const int daylightOffset_sec = 0;

TaskHandle task;
FirebaseModule firebase;
FirebaseAuthentication auth;

enum LockerEnum {
  LOCKER_A,
  LOCKER_B,
  LOCKER_C,
  LOCKER_TOTAL,
};

////////// Sensor //////////

////////// Communication //////////
HardSerial usbSerial;
SwapSerial bluetooth;

////////// Input Module //////////
DigitalIn magnetic[LOCKER_TOTAL] = {
  DigitalIn(5),
  DigitalIn(18),
  DigitalIn(19),
};

////////// Output Module //////////
DigitalOut solenoid[LOCKER_TOTAL] = {
  DigitalOut(13, true),
  DigitalOut(27, true),
  DigitalOut(26, true),
};

DigitalOut cam[LOCKER_TOTAL] = {
  DigitalOut(25),
  DigitalOut(32),
  DigitalOut(33),
};

////////// Global Variable //////////
enum DebugLevel {
  DEBUG_NULL,  // 0
  DEBUG_IO,    // 1
  DEBUG_TIME   // 2
};

struct Products {
  DateTime endDate;
  String name;
  String openMsg;
  String closeMsg;
  bool status;
};

struct SystemVariable {
  Products locker[LOCKER_TOTAL];
  int firebaseReady = 0;
  int magneticState[LOCKER_TOTAL];
  int magneticStateBefore[LOCKER_TOTAL];
  DateTime nowDate = DateTime("2000-01-01T00:00:00");
  int debugLevel = 0;
};

SystemVariable var;