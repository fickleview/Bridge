// MRMP Wireless and Serial Bridge development environment
// c 2014-04  S. George Matthews   mrmp@fickleview.com
// Last Modified 2015-04-17 2115J

#define kRelease "2.1.210"             // Used by MRMP to determine capabilities
#define BUILD_UNIX_TIME 1397780087     // Used for UNIX time sanity checks


#include <Arduino.h>
#include <Streaming.h>   // Serial.print alternative
#include <stdlib.h>      // String functions
#include <Timer.h>
#include <SM.h>

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

#include "AccelStepper.h"




/*
Recent Build notes 
*/


