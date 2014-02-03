
// MRMP Wireless and Serial Bridge development environment
// c 2013-03  S. George Matthews   mrmp@fickleview.com

#define kRelease "2.0.2.001"           // Used by MRMP to determine capabilities
#define BUILD_UNIX_TIME 1391368815     // Used for UNIX time sanity checks


#include <Arduino.h>
#include <Streaming.h>   // Serial.print alternative
#include <stdlib.h>      // String functions
#include <Timer.h>

// These should not be modified. Options defined on MRMP.h


#include "ControllerSketch.h"

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





/*
Recent Build notes 
*/


