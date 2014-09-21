// Last modified 2014-09-01 
// Should not be modified


// DS18B20 

OneWire ds18b20(DS18B20_PIN);  // OneWire is a Dallas Semiconductor library

// DS18B20 Statemachine implementation

// SM_DS18B20.Set(M_start_DS18B20); // Do that when you want to collect temps

State SM_start_DS18B20()
{
 //*DS18B20addr   // Start using pointer
 
       ds18b20.reset();                  // reset the bus, no error detection
       ds18b20.select(DS18B20addr);     // Now all ROM commands address this device until next reset
       ds18b20.write(0x44, 1);           // Start conversion on selected device
       
       SM_DS18B20.Set(SM_wait_DS18B20);  //
         
}

State SM_wait_DS18B20()
{
 if(SM_DS18B20.Timeout(1000)) 
 {
  int iDS18BTemp  =0;
  int iDS18B20bits=0;
  byte data[12];
 
   ds18b20.reset();
   ds18b20.select(DS18B20addr);
   
   // Issue Read scratchpad command
   ds18b20.write(0xBE);
   
  // Receive 9 bytes
   for ( int i = 0; i < 9; i++) 
   {
   data[i] = ds18b20.read();
   }
     
 // Calculate temperature value  
   iDS18B20bits = ((data[0] & 0xf) * 625);  // Decimal part *1000
  //  Serial << "Decimal: " << iDS18B20bits << endl;
  //  Serial << "Modulo : " << iDS18B20bits % 1000 << endl;
   int iDS18B20DecBit = iDS18B20bits / 1000;
   if ((iDS18B20bits % 1000) >= 750)
   {
    iDS18B20DecBit +=1; // round up
   }
   
   iDS18BTemp=((word(data[1],data[0])) >> 4)*10;
   iDS18BTemp=iDS18BTemp+iDS18B20DecBit;
   
   switch(DS18B20addr[1])
   {
    default:
    SM_DS18B20.Finish();
    *insideTemp305 = -32000;
    *outsideTemp306 = -32000;
    
    break;
    
   case (INSIDE_DS18B20_SENSOR):
    *insideTemp305 = iDS18BTemp;
     DS18B20addr = DS18B20_BRIDGE_OUTSIDE_ADD;
     SM_DS18B20.Set(SM_start_DS18B20); // Do outside
   break;
  
   case (OUTSIDE_DS18B20_SENSOR):
    *outsideTemp306 = iDS18BTemp;
     DS18B20addr = DS18B20_BRIDGE_INSIDE_ADD;
     SM_DS18B20.Finish();  // In another function 
   
   break;
  
  
   }
 }
}

// end State machine DS18B20 


/*
// DS18B20 test script

#define DS18S20_ID 0x10
#define DS18B20_ID 0x28

// OneWire ds18b20(DS18B20_PIN);  // OneWire is a Dallas Semiconductor library
  
 float fTempDS18B20;
 int   iTempDS18B20;
 int iLastTempDS18B20 = 0;

 boolean getTemperature()
 {
  byte i;
  byte present = 0;
  byte data[12];
  byte  addr[8];
  
 //find a device
   if (!ds18b20.search(addr)) 
   {
    ds18b20.reset_search();
    return false;
   }
   
   // Print the address in HEX
   Serial << "Found DS: ";
   
    for( i = 0; i <= 8; i++) 
    {
    Serial << _HEX(addr[i]);
    }
    Serial << endl;
    
   if (OneWire::crc8( addr, 7) != addr[7]) 
   {
   return false;
   }
   
   
   if (addr[0] != DS18S20_ID && addr[0] != DS18B20_ID) 
   {
   return false;
   }
   
   ds18b20.reset();
   ds18b20.select(addr);
   
 // Start conversion
   ds18b20.write(0x44, 1);
   
 // Wait some time...
   delay(850);
   present = ds18b20.reset();
   ds18b20.select(addr);
   
 // Issue Read scratchpad command
   ds18b20.write(0xBE);
   
 // Receive 9 bytes
   for ( i = 0; i < 9; i++) 
   {
   data[i] = ds18b20.read();
   }
   
   
 // Calculate temperature value
   fTempDS18B20 = ( (data[1] << 8) + data[0] )*0.0625;
   iTempDS18B20 = int(fTempDS18B20);
 
 
 
  
// Send a notification out the serial port
// Log it as well. Has to be to and from 'X'..   116 is 't' text - low severity
// Humidity is '100' in the example

 if(!(iLastTempDS18B20==iTempDS18B20))
 {
  iLastTempDS18B20=iTempDS18B20;
 
   // Serial << F("DS18B20 status okay.") << endl;
    Serial << F("Temp float: ") << fTempDS18B20 << endl;
    Serial << F("Temp Int  : ") << iTempDS18B20 << endl;
    
//   Serial << F("RXXB*N4,116,") << iTempDS18B20 << F(",100,")  << *absolute_time_t501 << "#" << endl;
 }
  return true;
}

*/


// TTTTTTTTTTTTTTTTTTTT Temperature 2.0 TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
// TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT

#ifdef TEMPERATURE_LM335

#define BitsPerDegree 215       // Slope  2.15
// #define Bits0Degrees 5490       // Offset. 10 x the bits at zero degrees in WATER. 
#define Bits0DegreesAir 5650    // Offset. 10 x the bits at zero degrees AIR temp.
// When self heating is an issue for air temp make this lower.


// Read the temperature LM335 sensor 2k2 and 5v
//  makes the last digit round down, 5, or  round up
 int roundTemp(int t)
 {
 int m = 0;  // Modulo
 
 m = t % 10;  // What is the last bit?
 t -= m;      // Strip the last bit
 
     if (m >= 7)
       {
       m = 10;            // Round up
       }
         else if (m <= 3)
           {
           m = 0;        // Round down
           }
 
       else
       {
       m = 5;            // Make it .5   
       }
       return (t += m);      // Now make the last digit round down, 5, or  round up
           
 }
 

// Read the temperature LM335 sensor 2k2 and 5v
int tempOnPin(int _pin,int _offset,int _slope)
{    

  int _temp;

  _temp = ((analogRead(_pin) - _offset) * 100 / _slope);      //  10 x the degrees c on pin returned

      if ((_temp > MaxTempError) || (_temp < MinTempError))   // between Max and Min or error
      {
        return (TempError) ;     // Used by calling routine to determine if sensor is shorted or open
      }
      else
      {
       return (roundTemp(_temp));
    
      }
}


long getTemp()   // fieldType = FieldBase, field = sensor or channel
{
  switch (fieldType) 
  {
    case (OTHER_BASE):   // Currently ony 900 range
    //

    // pin  = field // 0 to 5 else error
    // offset= atoi(stData + (dataFieldsize + 1 ) )  // each temp channel has a custom offset
    //
    // 1 = 5574 air
    // 0 = 5670 water

    // slope = BitsPerDegree
    // tempOnPin = 10x the degrees c, which peovides a single decimal point resolution.
    // roundTemp = last digit rounded down to 0, 5 or rounded up to next.

    if ((field >= 0) && (field <= 15))  // Valid analog pins for temperature
    {
      errorMRMP = '0';
      return (tempOnPin(field,parm2,BitsPerDegree)) ;
    }
    else
    {
       errorMRMP = 't';
      return TempError;
    }

    break;


  default: 
    // RpacketReply('t',0);
    errorMRMP = 't';
    return TempError;
  }
}

#endif // TEMPERATURe_LM335

