// Built on 2.1.001 
// Should not be modified

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

  
#define MaxTempError 2000       // Used to set upper and lower before error value passes
#define MinTempError -500
#define TempError  -32000       // Used by functions to determine if sensor shorted or open

// Prototypes
extern int tempOnPin(int,int,int);
extern long getTemp();

#endif
