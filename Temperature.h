// Last modified 
// Should not be modified

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

  
const int MaxTempError =2000;       // Used to set upper and lower before error value passes
const int MinTempError =-500;
const int TempError  =-32000;       // Used by functions to determine if sensor shorted or open

// Prototypes
extern int tempOnPin(int,int,int);
extern long getTemp();

#endif
