// Last Modified 2015-04-21 1001J
// Should not be modified
// See userSetp on ControllerSketch for your setup code.
 
// *************** Setup ***************************
// *************************************************

void setup() 
{
 
  Serial.begin(57600);

   int _timerEvent1;
   int _timerEvent2;



  _timerEvent1 = timer.every(1000, tickTock);  // Update the clock every second
  _timerEvent2 = timer.every( 100, task10xS);  // Called 10x per second
  

#ifdef SOFTSERIAL_BRIDGING
  SerialBridge1.begin(57600);
#endif


   Serial << endl << F("THIS_DEV:") << THIS_DEV << F(" Release:") << kRelease  << endl;
   Serial << F("Free memory:") << freeRam() << endl << endl;

 Serial << F("EEPROM defined in MRMP.h for: ");
 
#ifdef ATMEGA2560   //  4k EEPROM
 Serial << F("ATMEGA2560 4k");
#endif

#ifdef ATMEGA328   //  1k EEPROM
 Serial << F("ATMEGA328 1k");
#endif

#ifdef ATMEGA168   //  .5k EEPROM
 Serial << F("ATMEGA168 .5k");
#endif
Serial  << endl  << endl;


#ifdef LCD_DISPLAY
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines
  
  lcd.backlight();         // backlight on  

// NOTE: Cursor Position: CHAR, LINE) start at 0  
  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print(THIS_DEV);
  
#endif
   
#ifdef WIRELESS_RF24
  // RF24 setup
   Serial << F("RF24 Using CE pin:") << CE_pin << endl << endl;


  radio.begin();  // RF24 wireless
 
  radio.enableDynamicPayloads();   // Increases efficency for small packets
 // radio.enableAckPayload();      // Not used - We would send the replies automatically on receipt

   radio.setAutoAck(false);        //B92 not used
 // optionally, increase the delay between retries & # of retries  delay,number
 // radio.setRetries(5,5);          // Default is best but may increase delivery to 20 mS
 radio.setDataRate(RF24_250KBPS);  // 250 has best range and low paket loss
 radio.setChannel(71);             // Set to a quiet channel for all devices


   //
   // Open listening pipe from other devices 
   //

for (int i=1; i<=kNumberOfPipes; ++i)
{
radio.openReadingPipe(i,pipes[i-1]); //B92
 // #ifdef DEBRF24RADIO
 unsigned long _pipe = pipes[i-1];
  Serial << F("Listening on pipe:") << _HEX(_pipe) << endl; 
 // #endif
  
}

pipe_num = 0;
radio.startListening();
  //
  // Dump the configuration of the rf unit for debugging
  //


  //   radio.printDetails();  // Needs printf
 
  

#endif  // WIRELESS_RF24



delay(random(0, 1000));  // To prevent all device queries simultaneously at start on global power recovery

#ifdef FETCH_UNIX_TIME
tickTock();              // setup baseline clock values

readUNIXtime();          // from UNIXTIME_ON_DEV 
#endif




#ifdef PING
     pinMode(PING_PIN, OUTPUT); // Set the pin for Ping with LED blink.
digitalWrite(PING_PIN, HIGH);
#endif


// Comment these out once initially burned.

#ifdef EEPROM_ARRAYS
/*  // Do this with a fresh EEPROM
RecordWriteINT(EEPROMadd3xx(0), -5);  // Time Zone burned
RecordWriteINT(EEPROMadd3xx(1),  0);  // Weekday incremented automatically each day
RecordWriteINT(EEPROMadd3xx(2),  0);  // Date  incremented automatically each day and usually remotely set

RecordWriteBYTE(EEPROMadd2xx(1),  3);  // reply packet Debug level set to 3 by default
*/


arrayLoadFromEEPROM2nn(0,2);  // First and Last record , not used,   DebugLevel[201]

arrayLoadFromEEPROM3nn(0,2); // First and Last record,      TZ[300],     Weekday[301],  Date{302]
#endif



#ifdef DEBUG_EEPROM_RECORDS
  // Dump EEPROM recorder records. (NOT the array)
 
 Serial << endl << endl << "Index:Value - A dump of EEPROM records."  << endl;
 int _l = RecordReadEPROMint(RECORD_LAST_NOTIFY);
 int _p = RecordReadEPROMint(RECORD_POINTER);
 
 Serial << "Last notify: " << _l << " Record pointer: " << _p << endl << endl;

  for (int i=0; i <= ((RECORD_LAST - RECORD_FIRST) / RECORD_LENGTH); i++)
  {
    RecordReadEEPROM(i);
  }

#endif // DEBUG_EEPROM_RECORDS


#ifdef EEPROM_RECORDS
// Reset the notification pointer to avoid dumping all records at startup
   if(abs(RecordReadLastNotify() - RecordLastEEPROMwritten()) > 10)
  {
    RecordWriteLastNotify(RecordLastEEPROMwritten());
  }
 
 Serial << endl << endl << F("Last notify set to 1 in StartupAndLoop") << endl << endl; 
  RecordWriteLastNotify(1);  // Test
  
#endif // EEPROM_RECORDS



  // Reset code, and random user codes. Comment these two once set.
  // 0-6 code.  Unused replace with \0  then 8th is group.
  // 'c','o','d','e','\0','\0','\0','a', 

group = '\0';
  
#ifdef SECURITY
// Create and burn a default passcode at startup. Comment out once burned.
  char userCode[8] = {
    '4','a','v','r','s','c','p','a',                                                                                                                                                                                          };
  for (int i=0; i <= 7; i++)
  {
    SecurityWriteCodeByte(1, i, userCode[i]);
  }
#endif
  /*

   Serial.print(" Group: ");
   Serial.println(SecurityReadGroup(1));
   
   
   // Wipe users passcodes. Comment out in production. 
   
   randomSeed(analogRead(6));   // Specify an unconnected pin.
   for (int i=8; i <= 63; i++)
   {
   // SecurityWriteCodeByte(i / 8, i % 8,'\0');
   SecurityWriteCodeByte(i / 8, i % 8,random(32,126));
   }
   
   */

  /*
  // Dump passcodes.  Debug
   char c;
   Serial.println();
   for (int i=0; i <= 63; i++)
   {
   if (!(i % 8))
   {
   Serial.println();
   Serial.print((i / 8) + 1 );
   Serial.print('>');
   }
   c = SecurityReadCodeByte((i / 8) + 1 , i % 8);
   if (c < 32)
   {
   Serial.print(' ');
   }
   else
   {
   Serial.print(c);
   }
   
   }
   Serial.println();
   
   */


  // Examples of EEPROM initial burn - can be burned with MRMP 'e' as well Rg0vgP3,300,111,e#
  // Comment out once burned or defaults in RAM will overwrite EEPROM values.
  /*
char e;
   
   Serial.println();
   Serial.print("Burn EEPROM Startup errors: ");
   e = arrayToEEPROM3nn(0, *example300);
   Serial.print(e);
   e = arrayToEEPROM3nn(1, *example301);
   Serial.print(e);
   
   
   e = arrayToEEPROM5nn(1, 1224207552);  // UNIX time 2008-10-16T21:30 -5
   Serial.print(e);
   Serial.println();
   */



  // Examples of EEPROM initial read during Setup. Can be read with MRMP 'e' as well Rg0vgG2,300,e#

  // A range...

  //   arrayLoadFromEEPROM3nn(0,5);  // First and Last record 
  //   arrayLoadFromEEPROM5nn(2,3);  // First and last record.  0 and 1 are UNIX time.

  /*  
   
   // or individually...
   
   *example300 = arrayFromEEPROM3nn(0);  // Fields must match. 300 = 0
   *example301 = arrayFromEEPROM3nn(1);  //                    301 = 1
   
   
   
   */




  /* 
   // Fill EEPROM records with a series values
   byte e;
   Serial.println();
   Serial.print("Filling EEPROM int: ");
   for (int i = 0; i <= ARRAY_TO_EEPROM_MAX / 2; i++)
   {
   e = arrayToEEPROM3nn(i,i + 300);
   Serial.print(e);
   }
   
   
   // Fill EEPROM records with a series values
   
   Serial.println();
   Serial.print("Filling EEPROM long: ");
   for (long i = 0; i <= ARRAY_TO_EEPROM_MAX / 2; i++)
   {
   e = arrayToEEPROM5nn(i,i + 5000000);
   Serial.print(e);
   }
   
   */


  
  // Dump EEPROM int records  - Debug
#ifdef DEBUG_EEPROM_ARRAYS
  Serial << endl << endl << F("Index:Value - A dump of EEPROM arrays. None or some may be loaded at Startup or by your functions.") << endl << endl;
 int j = 0;
  Serial << F("EEPROM byte 2nn: ") << endl;
  for (int i = 0; i < BYTE_FIELD_SIZE; i++)
  {
    ++j;
     Serial << i << ":"  << arrayFromEEPROM2nn(i) << ", ";
    if( j > 19)
    {
    j = 0;
    Serial << endl;
    }
  }
 Serial << endl << endl;
 

 j = 0;
  Serial << F("EEPROM int 3nn: ") << endl;
  for (int i = 0; i < INT_FIELD_SIZE; i++)
  {
    ++j;
    Serial << i << ":"  << arrayFromEEPROM3nn(i) << ", ";
    if( j > 19)
    {
    j = 0;
    Serial << endl;
    }
  }
 Serial << endl << endl;

 j = 0;
  Serial << F("EEPROM long 5nn: ") << endl;
  for (int i = 0; i < LONG_FIELD_SIZE; i++)
  {
     ++j;
    Serial << i << ":"  << arrayFromEEPROM5nn(i) << ", ";
    if( j > 19)
    {
    j = 0;
    Serial << endl;
    }
  }
 Serial << endl << endl;
  #endif // EEPROM_ARRAYS
 




 #ifdef WATCHDOG
       pinMode(WATCHDOG_PIN, OUTPUT);  // No need for pullup as we supply an external to bias the SS Realy noormally on.
  digitalWrite(WATCHDOG_PIN, HIGH);    // Enable power at startup


  *watchDogSnooze303   = 1200 ;        // Twenty minutes. The CRON tab that updates *watchDogLastBark502 must be less than this
  *watchDogLastBark502 = 2147000000;   // Put the dog to sleep until 2038-01 if no external cron UNIX time to start.
  *watchDogState304    = 0;            // Disabled at statup!
#endif

  userSetup();  // See userSetp on ControllerSketch for your setup code.


}  // Setup



// %%%%%%%%%%%%%%%%%%%  Loop %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

void loop() 
{
  
    timer.update();
    mainTasks();
    userTasks();
    
  // delay(10);

}
