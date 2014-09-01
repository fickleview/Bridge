
// MRMP Wireless and Serial Bridge development environment
// c 2014-04  S. George Matthews   mrmp@fickleview.com
// Last Modified 2014-08-30 0935J

#define kRelease "2.3"             // Used by MRMP to determine capabilities
#define BUILD_UNIX_TIME 1409402147 // Used for UNIX time sanity checks


#include <Arduino.h>
#include <Streaming.h>   // Serial.print alternative
#include <stdlib.h>      // String functions
#include <Timer.h>
#include <SM.h>
#include <OneWire.h>

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

#include "Temperature.h" 
#include "Volts.h"



/*
Recent Build notes 
*/


