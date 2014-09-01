// Last modified 
// Should not be modified

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

// DS18B20 using known addresses

 uint8_t DS18B20_BRIDGE_INSIDE_ADD[8]  = {0x28,0x91,0x62,0x8e,0x5,0x0,0x0,0xf3};  // Sun room inside 

// Marked: 8f 
uint8_t DS18B20_BRIDGE_OUTSIDE_ADD[8]  = {0x28,0xe6,0x65,0x48,0x5,0x0,0x0,0x8f};  // Sun room outside 

int iDS18BTempSunroom;
int iDS18BTempOutside;

  byte      DS18b20data[12];
  long      lDS18b20ConvertStartedAt = 0;    // For Time out
  const int CONVERSION_TIME_MS       = 2000; // Time out conversion time typicall less

int iGetTemps = 0;

  
const int MaxTempError =2000;       // Used to set upper and lower before error value passes
const int MinTempError =-500;
const int TempError  =-32000;       // Used by functions to determine if sensor shorted or open

// Prototypes
extern int tempOnPin(int,int,int);
extern long getTemp();

#endif
