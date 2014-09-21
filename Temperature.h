// Last modified 
// Should not be modified

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

extern State SM_start_DS18B20();
extern State SM_wait_DS18B20();


SM SM_DS18B20(SM_start_DS18B20, SM_wait_DS18B20);//create statemchine with initial head and body state functions



// DS18B20 using known addresses

 uint8_t DS18B20_BRIDGE_INSIDE_ADD[8]  = {0x28,0x91,0x62,0x8e,0x5,0x0,0x0,0xf3};  // Sun room inside 
 const uint8_t INSIDE_DS18B20_SENSOR = 0x91 ; // Unique to all devices

// Marked: 8f 
uint8_t DS18B20_BRIDGE_OUTSIDE_ADD[8]  = {0x28,0xe6,0x65,0x48,0x5,0x0,0x0,0x8f};  // Sun room outside 
const uint8_t OUTSIDE_DS18B20_SENSOR = 0xe6 ;

uint8_t *DS18B20addr = DS18B20_BRIDGE_INSIDE_ADD;  // Initially points to the first DS18B20 address

  long      lDS18b20ConvertStartedAt = 0;    // For Time out
  const int CONVERSION_TIME_MS       = 2000; // Time out conversion time typicall less

int iGetTemps = 0;


// LM335 sensor
const int MaxTempError =2000;       // Used to set upper and lower before error value passes
const int MinTempError =-500;
const int TempError  =-32000;       // Used by functions to determine if sensor shorted or open

// Prototypes
extern int tempOnPin(int,int,int);
extern long getTemp();

#endif
