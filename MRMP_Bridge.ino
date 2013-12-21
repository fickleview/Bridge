
// MRMP Wireless and Serial Bridge development environment
// c 2013-03  S. George Matthews   mrmp@fickleview.com

#define kRelease "2.0.1.210"           // Used by MRMP to determine capabilities
#define BUILD_UNIX_TIME 1365184323     // Used for UNIX time sanity checks


#include <Arduino.h>
#include <Streaming.h>   // Serial.print alternative
#include <stdlib.h>      // String functions

// These should not be modified. Options defined on MRMP.h


#include "MRMP.h" 
#include "RF24routing.h"
#include "MRMPfunctions.h"
#include "MRMParrays.h"

#include "MRmPmachinePackets.h"
#include "EEPROM.h" 
#include "MRMPfunctions.h"

#include "RF24setup.h"
#include "Time.h"

#include "DNHT11_Temp_Humidity.h"

#include "Temperature.h" 
#include "Volts.h"


#include "ControllerSketch.h"


/*
Recent Build notes 
*/


