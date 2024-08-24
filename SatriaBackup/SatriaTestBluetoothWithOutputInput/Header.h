#define ENABLE_MODULE_SERIAL_HARD
#define ENABLE_MODULE_SERIAL_SWAP
#define ENABLE_MODULE_DIGITAL_INPUT
#define ENABLE_MODULE_DIGITAL_OUTPUT

#include "Kinematrix.h"

////////// Utility //////////

////////// Sensor //////////

////////// Communication //////////
HardSerial usbSerial;
SwapSerial bluetooth;

////////// Input Module //////////
DigitalIn magnetic1(5);
DigitalIn magnetic2(18);
DigitalIn magnetic3(19);

////////// Output Module //////////
DigitalOut solenoid1(13, true);
DigitalOut solenoid2(27, true);
DigitalOut solenoid3(26, true);

DigitalOut cam1(25);
DigitalOut cam2(32);
DigitalOut cam3(33);

////////// Global Variable //////////
