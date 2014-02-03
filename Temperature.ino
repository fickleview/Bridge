// Built on 2.1.001
// Should not be modified



// TTTTTTTTTTTTTTTTTTTT Temperature 2.0 TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
// TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT

#ifdef TEMPERATURE

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

#endif // TEMPERATURE

