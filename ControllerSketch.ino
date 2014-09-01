// Last modified 
// Your controller code here

OneWire ds18b20(DS18B20_PIN);  // OneWire is a Dallas Semiconductor library
  
void userSetup()  // Called once during setup avoid modifying SetuAndLoop
{
 
}


void userTasks() // Called during every loop. Avoid tasks that block.
{
 
}



void userCommand(char _command ) // a means to decouple the supplied command set from the user command set
{
  switch (_command )
  {
   default:  RpacketReply('n',_command);
  }
  
}  // userCommand
    
    
    
void userHandleNotification() // Handle notification packet sent by remote controller
{
     RpacketReply('0',reply); // Will confirm receipt of notification and handleTrackingTag will be called by remote controller.

    
#ifdef DEBUG_MACHINE_PACKETS  
Serial << endl << F("Replied to notification and got... ") << endl << F(" commandParameters:") << commandParameters << F(" parm1:") << parm1 << F(" parm2:") << parm2 << F(" parm3 ") << parm3 << F(" parm4 ") << parm4 << endl;
#endif // DEBUG_MACHINE_PACKETS 


              switch (parm1)  // Parameter 1 used to set notification levels
              {
                case 'a':  // High level alert audio and visual
                #ifdef DEBUG_MACHINE_PACKETS
                Serial << F("Audio and Visual notification") << endl;
                #endif // DEBUG_MACHINE_PACKETS              
                break;
                
                case 'v':  // Med level alert  visual
                #ifdef DEBUG_MACHINE_PACKETS
                Serial << F("Visual notification") << endl;
                #endif // DEBUG_MACHINE_PACKETS
                break;
                
                case 't':  // Low level alert text
                #ifdef DEBUG_MACHINE_PACKETS
                Serial << F("Text notification") << endl;
                #endif // DEBUG_MACHINE_PACKETS
                break;
                
                default:
                #ifdef DEBUG_MACHINE_PACKETS
               Serial << F("Default notification") << endl;
               #endif // DEBUG_MACHINE_PACKETS
                 break;
              }
               Serial <<  MRMPRecPacketBuffer << endl; // Always send the received notification to serial port for processing
               
}  // userHandleNotification


    
// Any task you have in your sketck may be executed periodically in these functions
// add to but avoid modifying...
void task10xS()
{

}


void taskEverySecond()
{
  
 switch (iGetTemps)
  {
   
   case (1):
   getTempFrom(DS18B20_BRIDGE_INSIDE_ADD);
   break;
   
   case(2):
   getTempFrom(DS18B20_BRIDGE_OUTSIDE_ADD);
   break;
   
   case(3):
   Serial << F("RXXB*N4,116,") << *insideTemp305 << "," << *outsideTemp306 << ","  << *absolute_time_t501 << "#" << endl;

   iGetTemps= 0;
   break;
  }


  if(!(*absolute_time_t501 % 10))  // every ten seconds
  {
    

 
   if(NOTIFY_DEV != THIS_DEV)
   {
    PollRecordReadEEPROMtoNotify();
   }
 
   // Log Temp and Humd. Has to be to and from 'X'..   116 is 't' text - low severity
   //   Serial << F("R") << NOTIFY_DEV << THIS_DEV << F("B*N4,")  << _level << F(",") << _tempAv << F(",")  << _humd <<  F(",")  << *absolute_time_t501 << "#" << endl;

  
  }  // every ten seconds
  
  
  
  if (lastProcessTime+2 < *time_t500)
  { 
    processing = 'N';  // in case we get no EOP_CHAR at end of packet
  }
}



void taskEveryMinute()
{

 
  if(!(*absolute_time_t501 % 600))  // every ten minutes
  {
      iGetTemps = 1; // A series
 
  }
  
  
  if(hourIs() == 4 && minuteIs() == 2) // TZ changes at 02:00 Spring, 03:00 Fall, Bridge updated by script every hour at xx:01
  {
        #ifdef FETCH_UNIX_TIME 
          readUNIXtime();      // UNIXTIME_ON_DEV    // where to fetch the UNIX time  if != thsDevice 
        #endif 
  } 


  #ifdef EEPROM_ARRAYS
if(hourIs() == 4 && minuteIs() == 3) // The following minute. Too much blocking delay with EEPROM to perform at readUNIXtime();
  {           
          RecordWriteINT(EEPROMadd3xx(0),  *TZ300);       // TZ
          RecordWriteINT(EEPROMadd3xx(1),  *WeekDay301);  // Weekday incremented automatically each day
      
          RecordWriteINT(EEPROMadd3xx(2),  *Date302);     // Date       
  }       
  #endif  // EEPROM_ARRAYS
   
          
   printTime();    
   Serial << F("Free mem:") << freeRam() << endl;
   
   
   #ifdef WATCHDOG
     watchDog();   // Will remove power from router and DSL if awaken.
   #endif
   
   
   
   //B85
   #ifdef FETCH_UNIX_TIME
     if(absolute_time_tULmSTracking[0] || *absolute_time_t501 < BUILD_UNIX_TIME)  // has not been fufilled during startup so keep trying.
     {
       readUNIXtime();
     }
   #endif 
   
   

 if (wirelessTimeLimit < *time_t500)  // Drop authentication with no activity after 10 minutes
 {
   user = 0;
   group = 0;
 }
 
}  // taskEveryMinute



void taskEveryHour()
{


      
}

  
void taskEveryMonth()
{
  // UNIX time divided by 2629743 = months since 1970
  // UNIX time divided by 31556926 = years since 1970
}

void taskEveryDay()
{

}
 
 
 
// Modify this to handle your machine tags replied to by remote controller
// ASCII_FIRST_TRACKING_TAG to ASCII_LAST_TRACKING_TAG range set on MRMPmachinePackets.h as 'A' to 'E'
// Larger range uses more memory

void handleTrackingTag()
{
  if(trackingTagNotEpired(MRMP_TagInStr))
  {
  switch(MRMP_TagInStr)
  {
 
    case('A'):  // Update UNIX time. Do Not modify,
    
            switch(parm2)
            {
              
           #ifdef FETCH_UNIX_TIME
            case (501): // UNIX time
                
                 if(MRMP_ErrorCodeInStr == '0' && parm3 > BUILD_UNIX_TIME)
                 {
                 #ifdef DEBUG_MACHINE_PACKETS
                 Serial <<  F("Update *absolute_time_t501 case:") << MRMP_TagInStr << F(" Field:") << parm2 << F(" Value:") << parm3 << endl;
                 #endif
                 
                   *absolute_time_t501   = parm3; 
                   *absolute_time_tUL601 = (unsigned long)parm3; 
      
     
                   requestUNIX_TZandDate();
                 
                 }
                 #ifdef DEBUG_MACHINE_PACKETS
                 else
                 {
                 Serial << F("ERROR Update *absolute_time_t501 case:") << MRMP_TagInStr << F(" Field:") << parm2 << F(" Value:") << parm3 << endl;
                 }
                 #endif
                 
             break; // 501
             
             case (300): // TZ
                 if(MRMP_ErrorCodeInStr == '0')
                 {
                   #ifdef DEBUG_MACHINE_PACKETS
                   Serial << F("Update *TZ300 case:") << MRMP_TagInStr << F(" Field:") << parm2 << F(" Value:") << parm3 << endl;
                   #endif
               
                   *TZ300 =  parm3; 
                   
                   Serial << timeJuliet() << F("J") << endl;
 
                 }
             break; // 300
             
             //B86
             case (301): // Week day
                 if(MRMP_ErrorCodeInStr == '0')
                 {
                   #ifdef DEBUG_MACHINE_PACKETS
                   Serial << F("Update *WeekDay301 case:") << MRMP_TagInStr << F(" Field:") << parm2 << F(" Value:") << parm3 << endl;
                   #endif
               
                   *WeekDay301 =  parm3; 
                   
                 }
             break; // 301
             
             
              case (302): // Date  
                  if(MRMP_ErrorCodeInStr == '0')
                  {          
                    #ifdef DEBUG_MACHINE_PACKETS
                     Serial << F("Update *Date302 case:") << MRMP_TagInStr << F(" Field:") << parm2 << F(" Value:") << parm3 << endl;
                     #endif
      
                    *Date302 = parm3;    
                  }     
               break;  // 302
         #endif //  FETCH_UNIX_TIME
         #ifdef PING
           
               case (PING_PIN): // Ping pin 6
                  if(MRMP_ErrorCodeInStr == '0')
                  {          
                    #ifdef DEBUG_MACHINE_PACKETS
                     Serial << F("Ping:") << MRMP_TagInStr << F(" Field:") << parm2 << F(" Value:") << parm3 << endl;
                     #endif
      
                   handlePings();
                  }     
               break;  // 302
            #endif // PING    
             
              }  // parm2
    
    break;  // 'A'

      
      
    case('B'):
     #ifdef DEBUG_MACHINE_PACKETS
     Serial << F("Tracking tag case:") << MRMP_TagInStr << endl;
     #endif
     
      if(MRMP_ErrorCodeInStr == '0')  // Increment
                 {
                 #ifdef DEBUG_MACHINE_PACKETS
                 Serial << F("Increment EEPROM last:") << RecordReadLastNotify() << endl;
                 #endif
                 
                 incrementRecordWriteLastNotify();
                 }
     
    break;  // 'B'
    
    
    
    
    case('C'):
     #ifdef DEBUG_MACHINE_PACKETS
     Serial << F("Tracking tag case:") << MRMP_TagInStr << endl;
     #endif
     
         if(MRMP_ErrorCodeInStr == '0')  // Increment
                 {
                 #ifdef DEBUG_MACHINE_PACKETS
                 Serial << F("Room temperature from X:") <<  F(" Field:") << parm2 << F(" Value:") << parm3 << endl;
                 #endif
                 
                 incrementRecordWriteLastNotify();
                 }
                 
                 
    break;  // 'C'
    
  
    
    
    case('D'):
    
     #ifdef DEBUG_MACHINE_PACKETS
     Serial << F("Tracking tag case:") << MRMP_TagInStr << endl;
     #endif
     
    break;  //  'D'
    
    
    
    
    case('E'):
    
         #ifdef DEBUG_MACHINE_PACKETS
     Serial << F("Tracking tag case:") << MRMP_TagInStr << endl;
     #endif
     
    break;  // 'E'
    
     
 
  }
 } // Tracking tag expired
}


// Sending machine packets...
// sendMachinePacketTo( char _tagrmp, char _toDev, char _type, char _command, char _parameters, int _parameter1, long _parameter2,long _parameter3, long _parameter4,char _EEPROM)

// How UNIX time is fetched. 'A' is the machine packet tag and the packet is time stamped and must be received before expiry. 
// TRACKING_TAG_EXIPRY_mS is set to 1.5 seconds18b20. Even though packets are 2mS per hop.

// A sent packet need not be tagged and tracked, but this method avoids setting up custom code to manage replies.

// sendMachinePacketTo('A', 'X', 'R', 'G', '1', 501, 0, 0, 0, 0, '0');  // That is the actual code used to fetch the UNIX time from 'X'

// If successful, it triggers additional machine packet request for the date and day






/*
#define toggle 9//pin to toggle led on and off
#define led 13

SM Simple(S1);//create simple statemachine

State S1(){
  digitalWrite(led, HIGH);//turn led on
  if(digitalRead(toggle)) Simple.Set(S2);//wait for toggle pin to go high and change state to S2
}

State S2(){
  if(!digitalRead(toggle)) Simple.Set(S3);//wait for toggle pin to go low and change state to S3
}

State S3(){
  digitalWrite(led, LOW);//turn led off
  if(digitalRead(toggle)) Simple.Set(S4);//wait for toggle pin to go high and change state to S4
}

State S4(){
  if(!digitalRead(toggle)) Simple.Set(S1);//wait for toggle pin to go low and change state to S1
}
*/


