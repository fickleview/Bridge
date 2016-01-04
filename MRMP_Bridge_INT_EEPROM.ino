
// MRMP Wireless and Serial Bridge development environment
// c 2015  S. George Matthews   mrmp@fickleview.com
// Last Modified 2015-09-22

#define kRelease "2.5"             // Used by MRMP to determine capabilities
#define BUILD_UNIX_TIME 1411387706 // Used for UNIX time sanity checks


#include <Arduino.h>
#include <Streaming.h>   // Serial.print alternative
#include <stdlib.h>      // String functions
#include <Timer.h>
#include <EEPROM.h>
#include <SM.h>          // State machine
#include <OneWire.h>
//#include <Wire.h>        // For LCD
//#include <LiquidCrystal_I2C.h>

// These should not be modified. Options defined on MRMP.h


#include "ControllerSketch.h"

#include "MRMP.h" 
#include "RF24routing.h"
#include "MRMPfunctions.h"
#include "MRMParrays.h"

#include "MRmPmachinePackets.h"
#include "MRMP_EEPROM.h" 
#include "MRMPfunctions.h"

#include "LCD.h"
#include "RF24setup.h"
#include "Time.h"

#include "Temperature.h"
#include "Volts.h"



/*
Recent Build notes 
*/


