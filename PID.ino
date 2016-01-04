
long lastInput = 0;
long ITerm = 0;
long outMax =  900000;  // Typicall 10,000 times the desired value
long outMin =  0;  

//  kp 1000, ki 50,  kd 250  Conservative
//  kp 4000, ki 200, kd 1000 Agressive

// Called by Timer passing SampleTimeSec same as Timer period
// ki, kp, kd should be at least 1000 * the sample time and all negative for reverse output
// Initialize by;
// lastInput = input;
// ITerm = = input;

// You should manually calculate the maximum expected values to ensure the long value does not overrflow
long PIDOutput(long input, long setPoint, long kp, long ki, long kd, int SampleTimeSec)
{
  
   ki *= SampleTimeSec;
   kd /= SampleTimeSec;
     
   ITerm += constrain((ki * (setPoint-input)),outMin,outMin);

   ITerm = constrain((kp * (setPoint-input)) + ITerm - ( kd * (input-lastInput)),outMin,outMin);
   
   lastInput = input;
   
   return ITerm;

 }

