// Last modified 2015-01-29
// Should not be modified

   /*
 Mcrocontroller Remote Management Protocol (MRMP) (Bridge authenticating) with routing to RF24
 A simple printable ASCII packet protocol for communicating with Atmel AVR
 
 */

#ifdef SOFTSERIAL_BRIDGING  // - A means to brige two devices via SoftSerial
    #include <SoftwareSerial.h>  // Serial bridge

    #define SOFT1_RX_PIN 8    // Not all pins support change interrupts, 
    #define SOFT1_TX_PIN 7   
    
    SoftwareSerial SerialBridge1(SOFT1_RX_PIN, SOFT1_TX_PIN);
    
    #define SERIAL_BRIDGE_1_DEBUG
    // SofSerial pipe ASCII address
#endif


// ============== MRMP functions ==========================
// =======================================================


void RpacketData(long data)   // Builds string ,1234   where 1234 is data
{

      *MRMPPacketBufferPointer = ',';
       MRMPPacketBufferPointer++;

  ltoa(data,MRMPPacketBufferPointer,10); 
  
  MRMPPacketBufferPointer = MRMPSendPacketBuffer+strlen(MRMPSendPacketBuffer);

}

void RpacketString()
{
       *MRMPPacketBufferPointer = ',';
       MRMPPacketBufferPointer++;
       //xxxx
       sstrcpy(MRMPPacketBufferPointer,MRMPstringDataArray,21);
       
       MRMPPacketBufferPointer = MRMPPacketBufferPointer+strlen(MRMPstringDataArray);
      
}


void ByePacketFrom(char _to, char _from)
{
    packetHeader('\0','B',_to,_from,'r');
    
   PacketFooterAndSend();
}

// c == command, t == type
void packetHeader(char _c, char _t, char _to, char _from, char _reply)
{
   
  MRMPSendPacketBuffer[MRMP_TYPE_INDEX]    = _t;
  MRMPSendPacketBuffer[MRMP_TO_INDEX]      = _to;
  MRMPSendPacketBuffer[MRMP_FROM_INDEX]    = _from;
  
  MRMPSendPacketBuffer[MRMP_TAG_INDEX]     = tagrmp;
  
  MRMPSendPacketBuffer[MRMP_REPLY_INDEX]   = _reply;
  
  MRMPPacketBufferPointer = MRMPSendPacketBuffer+MRMP_COMMAND_INDEX; 
  
      if(_t != 'B' && _t != 'K')  // Bye or Okay
      {
      *MRMPPacketBufferPointer = _c;
         MRMPPacketBufferPointer++;
      }
}



void PacketFooterAndSend()
{
  *MRMPPacketBufferPointer = EOP_CHAR;
   MRMPPacketBufferPointer++;
   
  *MRMPPacketBufferPointer = '\0';  

   writePacketToPipe(MRMPSendPacketBuffer);
    // MRMPSendByteCount=0;
}


void RpacketReply( char _error, long _data)
{
  char _reply ='r';
    if(WithinTrackingTagRange(tagrmp))
    {
      _reply = REPLY_TRACKING_TAG_CHAR;
    }
  
      if(*DebugLevel201 > 1 || _error > '0')  // Verbosity level 1 to 3 or error 
      {
      packetHeader('E', 'R',fromDev,toDev,_reply); 
      if(*DebugLevel201==2)
        {
       *MRMPPacketBufferPointer = '2';
        }
        else
        {
        *MRMPPacketBufferPointer = '4';
        }
        MRMPPacketBufferPointer++;
    
       *MRMPPacketBufferPointer = ',';
        MRMPPacketBufferPointer++;
        
        *MRMPPacketBufferPointer = _error;
        MRMPPacketBufferPointer++;
        
          RpacketData(parm1);  
          RpacketData(_data);
      
       if(*DebugLevel201 >= 3)
        { 
        *MRMPPacketBufferPointer = ',';
        MRMPPacketBufferPointer++;
        
      *MRMPPacketBufferPointer = EEPROMPrm;
        MRMPPacketBufferPointer++;
        }
            PacketFooterAndSend();
      } // Verbosity level 2 to 3 or error
      
  else if (*DebugLevel201==1)
  {
  okay();
  }
       
}



void okay()
{
   packetHeader('\0','K',fromDev,toDev,'r');
   
   *MRMPPacketBufferPointer = 'V';
    MRMPPacketBufferPointer++;
    
    RpacketData(*DebugLevel201);  // byte field 201 Verbosity level
    
    PacketFooterAndSend();

      wirelessTimeLimit = *time_t500 + 600; // 10 minute timeout and will reset
}



void getData()   // fieldType = FieldBase, field = element in the array
{
  
  switch (fieldType) 
  {

    case (BYTE_FIELD_BASE):   // byte
    //

    if (field <= BYTE_FIELD_SIZE )
    {
      if(EEPROMPrm == 'e')  // We want to read from EEPROM
      {
        
        #ifdef EEPROM_ARRAYS
         RpacketReply('0',arrayFromEEPROM2nn(field));
        #else
         RpacketReply('N',parm1); // EEPROM Not implemented
        #endif
         
      }
      else
      {  
         RpacketReply('0',*(byteField + field)) ;
      }

    }
    else
    {
      //group = '\0';
      RpacketReply('<',field);
    }


    break;  // byte




    case (INT_FIELD_BASE):   // int
    //

    if (field <= INT_FIELD_SIZE )
    {
      if(EEPROMPrm == 'e')  // We want to read from EEPROM
      {
        #ifdef EEPROM_ARRAYS
        RpacketReply('0',arrayFromEEPROM3nn(field));
        #else
         RpacketReply('N',parm1); // EEPROM Not implemented
        #endif
      }
      else
      {  

        RpacketReply('0',*(intField + field)) ;
      }

    }
    else
    {
     // group = '\0';
      RpacketReply('<',field);
    }
    //group = '\0';
    //user = 0;
    //tagrmp ='?';

    break;   // int
    
    

    case (LONG_FIELD_BASE):  // unsigned long
    //
  
     if (field <= LONG_FIELD_SIZE )
    {

      if(EEPROMPrm == 'e')  // We want to wrte to EEPROM
      {
        #ifdef EEPROM_ARRAYS
        RpacketReply('0',arrayFromEEPROM5nn(field));
        #else
         RpacketReply('N',parm1); // EEPROM Not implemented
        #endif
        
      }
      else
      {  
        RpacketReply('0', *(longField + field)) ;
      }
    }
    else
    {
     // group = '\0';
      RpacketReply('>',field);
    }

    break;  // unsigned long
    
    
    
//B82
 case (ULONG_FIELD_BASE):  // unsigned long
    //
  
     if (field <= U_LONG_FIELD_SIZE )
    {
      if(EEPROMPrm == 'e')  // We want to wrte to EEPROM
      {

         RpacketReply('N',parm1); // EEPROM Not implemented      
      }
      else
      {  
        RpacketReply('0', (long)*(UlongField + field)) ;
      }
    }
    else
    {
     // group = '\0';
      RpacketReply('>',field);
    }

    break;    // unsigned long
    
        
  default: 
    //group = '\0';
    RpacketReply('g',0);
  }

}




void putData()   // fieldType = FieldBase, field = element in the array
{
 
  switch (fieldType) 
  {

    case (BYTE_FIELD_BASE):   // byte
    //

    if(field <= BYTE_FIELD_SIZE)
    {

      if(EEPROMPrm == 'e')  // We want to wrte to EEPROM
      {
        #ifdef EEPROM_ARRAYS
         RpacketReply(arrayToEEPROM2nn(field, byte(parm2)),parm2);   // Saves 300 series integer specified
        #else
         RpacketReply('N',parm2); // EEPROM Not implemented
        #endif
      }
      else
      {
        if(parm2 <= 255) // Max size before rollover
        {
          *(byteField + field) = parm2;
          RpacketReply('0',parm2); // Acknowlege witha no error packet.
 
         
        }
        else
        {
          RpacketReply('p',parm2); // Greater than 255
        }

      }

    }
    else
    {
     // group = '\0';
      RpacketReply('p',parm1);
    }

    break;   // byte
    
    


    case (INT_FIELD_BASE):   // int
    //

    if(field <= INT_FIELD_SIZE)
    {

      if(EEPROMPrm == 'e')  // We want to wrte to EEPROM
      {
         #ifdef EEPROM_ARRAYS
        RpacketReply(arrayToEEPROM3nn(field, int(parm2)),parm2);   // Saves 300 series integer specified
         #else
         RpacketReply('N',parm2); // EEPROM Not implemented
         #endif
      }
      else
      {
        *(intField + field) = parm2;
        RpacketReply('0',parm2); // Acknowlege with a no error packet.
      }

    }
    else
    {
     // group = '\0';
      RpacketReply('p',parm1);
    }

    break;   // int
    
    

    case (LONG_FIELD_BASE):  // long
    //

    if(field <= LONG_FIELD_SIZE)   
    {
      if(EEPROMPrm == 'e')  // We want to wrte to EEPROM
      {
       #ifdef EEPROM_ARRAYS
        RpacketReply(arrayToEEPROM5nn(field, parm2),parm2);   // Saves 500 series integer specified
       #else
         RpacketReply('N',parm2); // EEPROM Not implemented
       #endif

      }
      else
      {
        *(longField + field) = parm2;
        RpacketReply('0',parm2); // Acknowlege witha no error packet.
      }
    }
    else
    {
     // group = '\0';
      RpacketReply('p',parm1);
    }

    break;  // long

//B82
 case (ULONG_FIELD_BASE):  // unsigned long
    //

    if(field <= U_LONG_FIELD_SIZE)   
    {
      if(EEPROMPrm == 'e')  // We want to wrte to EEPROM
      {
         RpacketReply('N',parm2); // EEPROM Not implemented
      }
      else
      {
        *(UlongField + field) =  (unsigned long)parm2;
        RpacketReply('0',parm2); // Acknowlege witha no error packet.
      }
    }
    else
    {
     // group = '\0';
      RpacketReply('p',parm1);
    }

    break;  // unsigned long
    


  default: 
    //group = '\0';
    RpacketReply('p',fieldType);
  }
}



#ifdef SECURITY
// =============== Secutity passcode =========================

void RpassCodeReply(int u)
{
  packetHeader(command,'R',fromDev,THIS_DEV,'r');
  
  *MRMPPacketBufferPointer = '3';
   MRMPPacketBufferPointer++;
    
     RpacketData(parm1);  

  *MRMPPacketBufferPointer = ',';
   MRMPPacketBufferPointer++;
   
    
      for (int i=0; i <= 6 ; i++) // Send byte by byte until null
      {
        char c;
        c = SecurityReadCodeByte(u,i);
        if (c > 32)  // Printable
        {
            *MRMPPacketBufferPointer = c;
             MRMPPacketBufferPointer++;
        }
    
      }

  *MRMPPacketBufferPointer = ',';
   MRMPPacketBufferPointer++;
      
  *MRMPPacketBufferPointer = SecurityReadGroup(u);
   MRMPPacketBufferPointer++;
      
  PacketFooterAndSend();

}


void getUserPassCode()
{
  if(fieldType == OTHER_BASE)  // 980 to 989 = 0 to 9 of which 1 to 8 are valid
  {
    field = field - OTHER_BASEAuth;  // 0 to 9 User ID
    if ((field > 0) && (field <= 8))  // Valid for UID 1 to 8
    {
      RpassCodeReply(field);
    }
    else
    {
      RpacketReply('c',field);

    }
  }
  else
  {
    RpacketReply('C',fieldType);
  }
}



void putUserPassCode()
{
  char c;
  if(fieldType == OTHER_BASE)  // 980 to 989 = 0 to 9 of which 1 to 8 are valid
  {
    field = field - OTHER_BASEAuth;  // 0 to 9 User ID
    if ((field > 0) && (field <= 8))  // Valid for UID 1 to 8
    {
      // Serial.print(" User: ");  // Debug
      // Serial.print(field);
      // Serial.print(" Writing user passcode...");
      // Serial.println();
      byte eopw = 0;
      for(int i = 0; i <= 6; i++)
      {
       if(eopw)
       {
         c = '\0';
       }
       else
       {
        c = *(parameterPointerArray[2] + i);
        
          if (c == ',')  // Enf Of PassWord  eopw
          {
          eopw = 1;
          c = '\0';
          }
       }
        //  Serial.print(c);

        SecurityWriteCodeByte(field, i, c);  // 7 alphaumeric pascode
        
      } 
      SecurityWriteCodeByte(field, 7, *(parameterPointerArray[3])); // one group
      okay();
    }
    else
    {
      RpacketReply('X',field);

    }

  }
  else
  {
    RpacketReply('X',fieldType);
  }
}

#endif  // SECURITY



void parseRecHeader()
{
 
  reply = MRMP_ReplyInStr;
//B61
//B65

    toDev =     MRMP_ToInStr;
    fromDev =   MRMP_FromInStr;
    tagrmp =    MRMP_TagInStr;
    
    packetType = MRMP_TypeInStr;
 
    
    command =   MRMP_CommandInStr;    
    reply =     MRMP_ReplyInStr;
    commandParameters = ASCIItoDEC1(MRMP_ParametersInStr);    // convert to decimal 6th packetByte
    

// Using a parameterPointerArray[9] to poke in the address of each parameter

 EEPROMPrm = '*';
 
    parameterPointerArray[0] = MRMPRecPacketBuffer+MRMP_PARAMAETERS_INDEX; // The number of parameters
 
   
   int _i=1; 
  for(MRMPPacketBufferPointer = MRMPRecPacketBuffer+MRMP_PARAMAETERS_INDEX+1; _i <= commandParameters; MRMPPacketBufferPointer++) 
  {
     if(*MRMPPacketBufferPointer==',')
     {
       parameterPointerArray[_i] = MRMPPacketBufferPointer+1;   // Parameter address pointer
       _i++;
      
     }
      else if(*MRMPPacketBufferPointer==EOP_CHAR)                // More parameters specified that exists
     {        
         break; 
     }
   }
 
    parm1 = 0;
    parm2 = 0;
    parm3 = 0;
    parm4 = 0;

    parm1 = strtol(parameterPointerArray[1],'\0',10);
  
    if(commandParameters>=2)
    {
      parm2 = strtol(parameterPointerArray[2],'\0',10);
      
          if(commandParameters>=3)
          {
            parm3 = strtol(parameterPointerArray[3],'\0',10);
            
             if(commandParameters>=4)
          {
            parm4 = strtol(parameterPointerArray[4],'\0',10);
          }
          }
    }
  

  
#ifdef DEBUGREC
 
Serial.print(" commandParameters:");
Serial.print(commandParameters);

Serial.print(" parm1:");
Serial.print(parm1);
Serial.print(" parm2:");
Serial.print(parm2);
Serial.print(" parm3 ");
Serial.print(parm3);
Serial.print(" parm4 ");
Serial.println(parm4);

#endif
  
  fieldType = 100 * (ASCIItoDEC1(*parameterPointerArray[1]));  // First byte
  field =  int(parm1 ) - fieldType;


}
void executeCommand()  // Commands built by processCommand will be executed
{
 int   _pin =0;
 int _value =0;
 
 parseRecHeader();
 
  if(command != 'A')  // Authenticate
  {
    if(group == 'a' || group == 'm' || group == 'r' || (MRMP_TagInStr == MACHINE_TAG) || WithinTrackingTagRange(tagrmp))  // Must be one of these groupsor a machine tag
    {

      switch (command) 
      {
        // A means to handle user supplied command sets in ControllerSketch
      default: userCommand(command );
     
       
        
      case 'E':  // Reply Error packet
      writePacketToPipe(MRMPRecPacketBuffer); // Parse and handle routing
      break;
      
      
      case 'G':  //Get data from arrays
       
        // The replay packet 
        // Any group can get
        //B62
        if(commandParameters==2)
        {
        EEPROMPrm = *parameterPointerArray[2];  // Second parameter e with EEPROM P or G
        }
        else
        {
          EEPROMPrm = 'r';
        }


        if(group || (MRMP_TagInStr == MACHINE_TAG) || WithinTrackingTagRange(tagrmp))
        {
          getData();
        }
        else
        {
          RpacketReply('A',MRMP_TagInStr);
        }
        break; //Get data from arrays

    case 'N':  // Notifications
    
    if(group =='a' || group == 'm' || (MRMP_TagInStr == MACHINE_TAG) || WithinTrackingTagRange(tagrmp)) // only admin or moderator group may write
        {
        
            if(commandParameters==4 )
            {
             userHandleNotification();          
            }
            else
            {
             RpacketReply('n',0); // Other format not currently accepted
            }      
        }
         else
        {
          RpacketReply('A',group);
        }

        break; // Notifications
     
         case 'D':  // Display
    
    if(group =='a' || group == 'm' || (MRMP_TagInStr == MACHINE_TAG) || WithinTrackingTagRange(tagrmp)) // only admin or moderator group may write
        {
        
            if(commandParameters==4 || commandParameters==5 )
            {
             handleDisplay();          
            }
            else
            {
             RpacketReply('n',0); // Other format not currently accepted
            }  
           RpacketReply('0','D'); // Acknowlege with a no error packet.    
        }
         else
        {
          RpacketReply('A',group);
        }

        break; // Display
     
 
  
     case 'P':  // Put data into array

        if(group =='a' || group == 'm' || (MRMP_TagInStr == MACHINE_TAG) || WithinTrackingTagRange(tagrmp)) // only admin or moderator group maywrite
        {
        
            if(commandParameters==3)
            {
            EEPROMPrm = *parameterPointerArray[3];  // Third parameter e with EEPROM P or G
            }
            else
            {
              EEPROMPrm = 'r';
            }

            putData();         // Put the data into the xxxField array
        
        }
         else
        {
          RpacketReply('A',group);
        }

        break; // Put data into array



 //B62 left here
      case 'C':  // Security
      
      #ifdef SECURITY
      
        // passCode  C1 is get, C2 is put
        Serial.print("Passcode ");  // Debug

        if( user == 1 && group == 'a')
        {
          if (commandParameters == 1)  // Get passcode 
          {
            Serial.print("Getting...");  // Debug

            getUserPassCode(); 
          }
          else   // Write passcode
          {
            Serial.print("Writing...");
            putUserPassCode();
          }

        }
        else
        {
          RpacketReply('C',user);
         
        }
        
         #else
          RpacketReply('N',parm2); // SECURITY Not implemented
         #endif
          

        break;  // Security


      case 'T': //Temperature data from sensors
               // The replay packet 

              errorMRMP = '?';
                // RX0**T1,901, 5564#  // other 901 to 909
                
                // RX0**T1,910#  //       910
                // RX0**T1,920#  //       920 thermal couple
          
        
         switch(field) // Records field
        {        // offset 5562
          
          
         default:
             RpacketReply('?', -32000);
             
             
         case 1:   // Other 
         case 2:
         case 3:
         case 4:
         case 5:
         case 6:
         case 7:
         case 8:
         case 9:
         #ifdef TEMPERATURE_LM335
         {
          int _valueGt = getTemp();
          RpacketReply(errorMRMP,_valueGt);
         }
         #else
         RpacketReply('o', -32000);
         #endif  // TEMPERATURE
         break;
         
         case 10:  // 
         
     
         break;
        

         
        }
        
         
        break;  //Temperature




        // Fetch a record. Replies with four packets and Okay. 
        // 100 to 1xx for specific fields
        // 200 for the last record

      case 'V': // Voltage
        /*  Rp1vV2,953,0#
         Returns x10 value.
         953 represents analog channel 3.
         0 represents the offset voltage. Typically '0', but may be set for battery monitors that typically have a 10v offset for lead acid batteries.
         
         */
         
         #ifdef VOLTS
          RpacketReply('0',getVoltsOnPin(constrain(0,49,field - 50), parm2));
         #else
          RpacketReply('N',parm2); // Volts Not implemented
         #endif
        
        break; // Voltage


      case 'B': // Bits
        /*  Rp1vgB1,2#
         
         Returns 0 to 1023.
         analog channel 2.
         
         ANALOG_PIN_MIN,ANALOG_PIN_MAX are accepted,  0 - 7 are typical limts
         */
         
         #ifdef ANALOG_RW
          _pin = constrain(parm1,ANALOG_PIN_MIN,ANALOG_PIN_MAX);
        _value = constrain(parm2,0,255);
         
              if(commandParameters == 2) // We want to write to the pin
              {    
                analogWrite(_pin,_value);
                RpacketReply('0',constrain(_value,0,255));
              }
              else
              {
                RpacketReply('0',analogRead(_pin));
              }
        
         #else
          RpacketReply('N',parm2); // Not implemented
         #endif
         
        break; // Bits
        
        

      case 'S': // State

        /*  Rp1vgS1,4#
         
         Returns True or False. 1 or 0.
         digital pin 4.
         
          DIGITAL_PIN_MIN,DIGITAL_PIN_MAX are accepted, typically representing pins 0 to 16 
         */
         //B68
         
         #ifdef DIGITAL_RW
         _pin   = constrain(parm1,DIGITAL_PIN_MIN,DIGITAL_PIN_MAX);
         _value = constrain(parm2,0,1);  // LOW or HIGH
        
        
              if(commandParameters == 2) // We want to write to the pin
              {
                digitalWrite(_pin,_value);
                RpacketReply('0' ,_value);
              }
              else
              {
                RpacketReply('0',digitalRead(_pin));
              }
        
         #else
          RpacketReply('N',parm2); // Not implemented
         #endif
              
        break; // State



      case 'R':  // Records

        // See MMRP documentation

        switch(field) // Records field
        {
          
        case 70:  // A single of record
          //  Serial.println(970); // Debug
             #ifdef EEPROM_RECORDS        
               RecordReadEEPROMtoString(parm2);
              // RecordReadEEPROMtoNotify(parm2); // Testing Notify format
             #else
               RpacketReply('N',parm2); // EEPROM records Not implemented
             #endif
    
          break; // A single of record


        case 71:  // Number of last record
          // Serial.println(971); // Debug
          #ifdef EEPROM_RECORDS    
             RpacketReply('0',(RECORD_LAST - RECORD_FIRST) / RECORD_LENGTH);
            #else
             RpacketReply('N',0); // EEPROM records Not implemented
            #endif
         
          break;  // Number of last record



        case 72:  // Number of last record written
          // Serial.println(972); // Debug
          
         #ifdef EEPROM_RECORDS 
              RpacketReply('0',RecordLastEEPROMwritten());        
              
         #else
          RpacketReply('N',0); // EEPROM records Not implemented
         #endif
         
          break;   // Number of last record written
     
         case 73:  // Free RAM
        
          RpacketReply('0',freeRam());
           break;
         
            
            
         case 74: // Ping device 'x' every mSeconds
            
           #ifdef PING
             pingEveryMSeconds(parm2, parm3);
           #else
             RpacketReply('N',parm2); // Not implemented
          #endif
          
            break;
            
         
          
        } // Records field

        break;   // Records
      }  // command

      commandParameters = 0; // 
      reply = 'n';
      
    }
    else   // Must be one of these groups or a machine tag
    {
      RpacketReply('A',group);
    }
  }
  else    // Authenticate
  {
  #ifdef SECURITY
    // 'A'  // Try to authenticate
   

    // group returned by  SecurityValidatePass();

    // printstData(); // Debug
    user = parm1;
    group = SecurityValidatePass(user);

    #ifdef DEBUG
    Serial << (" Authenticated... User: %i  Group: %c",user,group) << endl;  // Dbug
    #endif
    
    if(group)
    {
       okay();
     // RTPError('0'); // Send a packet that authentication is OKAY
    }
    else
    {
      user = 0;
      RpacketReply('q',group);
    }

  #else // Security always successful
    
    user = 1;
    group = 'a'; // Admin
    okay();
    
  #endif
    

  }      // Authenticate
  
reply = 'n';
}



// +++++++++++++++++ SCP main +++++++++++++++++++++
//       Serial Communication Protocol
// ++++++++++++++++++++++++++++++++++++++++++++++++
//
//                  Routing
// If this device is BRIDGE_X it is the Bridge for routing to/from controllers
// 
// If the toDev is = '0' then the data is sent out the local serial port
// 

void processSerial()
{
     if ((inByte < 127 ) && (inByte > 31 ))  // Only printable ASCII
    {
   // Poke the serial into the receive buffer
   //B61 consolidate RF24 and Serial Receive
            if(MRMPRecByteCount <96)
            {
             MRMPRecPacketBuffer[MRMPRecByteCount]=inByte; // poke in the next byte into the MRMP buffer
              MRMPRecByteCount++;
            }
            else
            {
          
            RpacketReply('>',MRMPRecByteCount);
            MRMPRecByteCount=0;
            processing = 'N'; // All chunks have been processed
           
            
             #ifdef DEBUGREC
             Serial << F(" Serial Packet overrflow \n\r");
             #endif
             
            }
            
          
         if (inByte == EOP_CHAR)
         {
            MRMPRecPacketBuffer[MRMPRecByteCount]=0;  // Null
    
            MRMPRecByteCount=0;
            processing = 'N'; // All chunks have been processed
            parseRecPacket();  //B61 entire string rather that character at a time
       
     }

    }
    else
    {
      inByte =  0;
    }
    
    
}
void getAvailableSerial()   // Check if any bytes are arriving on the serial ports
{
    //  #ifdef DEBRF24
    //   Serial << (" P=%c \n\r",processing);
    //   delay(200);
    //   #endif
       
  if ((processing == 'N') || (processing == '0'))   // Port '0' the UART
  {
 
    if (Serial.available()) 
    {
      processing = '0';   // grab it until EOP
      inByte = Serial.read();
       processSerial();
    }
  }
 //B79 
 #ifdef SOFTSERIAL_BRIDGING
   if ((processing == 'N') || (processing == '1'))  // Port '1' the SoftSerial port
  {
 
    if (SerialBridge1.available()) 
    {
      processing = '1';   // grab it until EOP
      inByte = SerialBridge1.read();
       processSerial();
    }
  }
  #endif
}

//B77 'X' is Bridge, '0'  typically local port '1' to '9' hosted ports
//  x = result is same local hosted port

char elementInRouteTable(char _Dev)
{
  char _element = 'N'; // 'N' Not found  'x' hosted port
     for(unsigned int _b=0; _b<sizeof(routeTable)-1; _b++)      // the table is comma delimited char pairs
     {
       #ifdef DEBUGREC
           Serial << " >< Route table size: " << sizeof(routeTable) << " Look for: " << _Dev << " Index: " <<  _b << endl;
       #endif
       
       if(_Dev == routeTable[_b])                // Scan for matching Controller toDev character
       {
  
        _element=routeTable[_b+1];
        
         #ifdef DEBUGREC
              Serial << " ><  Found:" << _element << endl;
          #endif
          
        _b=sizeof(routeTable)-1;
       }
       else   // scan ahead for a comma
       {
         while((routeTable[_b] != ',') && (_b < sizeof(routeTable)-1))
         {
           _b++;
         }
       }

     }      
     return _element;
}





void parseRecPacket() // Rather than process the packet a char at a time, process the entire string
{
     
  #ifdef DEBUGREC
       Serial << "Parsing: " << MRMPRecPacketBuffer << endl;
   #endif
   
   #ifdef DEBUG_RECEIVED_SENT_PACKETS
       Serial << endl << "DEBUG_RECEIVED: " << MRMPRecPacketBuffer << endl;
   #endif
   
   char _priorTag = tagrmp;
   
   
   
   lastProcessTime = *time_t500;
   
   parseRecHeader();
  
                
   //B77            
   // Determine if the packet is to be routed
   
   if(toDev != THIS_DEV)  
   {
      writePacketToPipe(MRMPRecPacketBuffer);
      //MRMPSendByteCount=0;
      
       #ifdef DEBUGREC
       Serial << "Routed To: " << toDev << " MRMPRecPacketBuffer: " << MRMPRecPacketBuffer << endl;
       #endif
   }
   else  // == THIS_DEV
   {
     //B60 
            #ifdef DEBUGREC
              Serial << "Handle it: " << MRMPRecPacketBuffer << endl;
            #endif
     
    

       //B83
         if ((packetType == 'R'  && _priorTag != tagrmp) && (tagrmp != MACHINE_TAG) && !WithinTrackingTagRange(tagrmp))    // Are we in a Realtime packet exchange?
        {
          // Is it the same tag as recorded previously?

            RpacketReply('1',0);
            tagrmp = '?';

        }
         else // valid packet
        {

        if (packetType == 'B')  // Bye? Then lets forget about this tag
        {
          // Then be polite and acknowledge with a Bye
          ByePacketFrom(fromDev, THIS_DEV);

          tagrmp = '?';
          group = '\0';
          user = 0;

        }
        else  // != 'B'  Bye
        {         
          // Check that the count is 0 to 9 ASCII
          if ((commandParameters >= 1) && (commandParameters <= MAX_PARAMETERS))
          {
            
            if(MRMPRecPacketBuffer[MRMP_PARAMAETERS_INDEX+1] == ',')  // command followed by a comma
              {
                //B61
            #ifdef DEBUGREC
              Serial << "Execute command: " << MRMPRecPacketBuffer << endl;
            #endif
            
            executeCommand();


              }
               else  // next char is a comma
              {
               RpacketReply('5',0); 
              }  // next char is a comma
            
            
           }
            else  // Not correct parameter range
           {
           
            RpacketReply('3',commandParameters); 

           } // Not correct parameter range
         
        }  // != 'B'  Bye
       }  // valid packket
         
  }   // == THIS_DEV
}
 

//$$$$$$$$$$$$$$$$$$$ Security 1.2 $$$$$$$$$$$$$$$$$$$$$$$$$
//ssssssssssssssssssssssssssssssssssssssssssssssssssssss

/* A simple pass code system.
 
 Authentication Records:
 
 Codes are 
 8 users x 8 = 64 byte block in EEPROM
 
 4 to 7 alphanumeric bytes.
 one 'Bridge' and 7 user codes.
 
 1 is Bridge
 2-7 is a user code.
 
 Last bye is user group, defined and assigned in MRMP during authentication.
 a = admin  // Only user that can change passcodes
 m = moderator (read, write)
 r = reader    (read)
 o = other
 
 The codes are passed as a string, stored and read as 7 individual bytes.
 Passcodes portion and group are stored in EEPROM.
 
 Bridge passcode is 1. Only Bridge can change passcodes and group.
 Bridge written to EEPROM in Setup.  Comment that out once the bridge is changed.
 
 Invite protocol:
 First paramenter is numeric digit for user, and the following 7 bytes is the pass code.
 
 User is 1, and passcode is fd1zf6
 Is0vA2,1,fd1zf6#
 
 Once validated, the invite is routed, replacing the passcode with the group read from EEPROM.
 
 Group is m.
 Is0vA2,1,m
 
 
 'C' passCode get and put
 When authenticated as usr 1, in group 'A'.
 RX0vgC1,981#
 C1 is get
 981 is user 1
 
 RX0vgC3,981,newcod,g#
 newcod is the hew passcode
 . is any blank after passcode until the group
 g is group.
 
 another
 RX0vgC3,981,newp,g#
 
 
 EEPROM event Recorder typically uses 383 to 511 block.
 The passcode must be in a free EEPROM block 0 to 382 ?
 
 
 If psscode is wrong on 3rd strke, you are ignored for 60 seconds.
 
 
 */

#ifdef SECURITY

#define SecurityPassAdd 318  // top free EEPROM 382 - ( 8 users * 8 bytes) = 318
#define SecurityPassLen 8    // the SecurityPassAdd x lenght x SecurityPassLen MUST NOT write over other EEPROM data
long strikeOutTime;          // On third strikeout, this is set to 60 seconds in te future
int strikeOutCount;

// User 1-8   ByteNum 0-7, byteNext 32 to 126
void SecurityWriteCodeByte(int user, int byteNum, char byteNext)
{
  EEPROM.write((SecurityPassAdd + (SecurityPassLen * (user - 1))) + byteNum,byteNext);
}

char SecurityReadCodeByte(int user, int byteNum)
{
  /*
  Serial.print('*');
   Serial.print(SecurityPassAdd + (SecurityPassLen * (user - 1)) + byteNum);
   Serial.print('*');
   */

  return EEPROM.read((SecurityPassAdd + (SecurityPassLen * (user -1))) + byteNum);
}

// Read the pass code stored in the receive buffer and compare to the stored code

// Last Byte in user record id their group ID
char SecurityReadGroup(int user)
{
  return SecurityReadCodeByte(user, (SecurityPassLen - 1));
}

// Return group or '\0' for group ID after authentication attempt using address of passcode address.
// Target passcodoe must be padded with '\0' (nulls) if less than 7 bytes.

int SecurityValidatePass(int user)
{
  if (strikeOutTime < *time_t500)  // Timer expired?
  {


    for(int i = 0; i <= 6; i++)
    {
      //B62
      #ifdef DEBUGAUTH
        Serial.print(SecurityReadCodeByte(user, i));
        Serial.print(*(parameterPointerArray[2] + i));
        Serial.println();
      #endif
      
      //B62 using parameterPointerArray[2]
      if(!(*(parameterPointerArray[2] + i) == SecurityReadCodeByte(user, i) ))
      {
        if(!SecurityReadCodeByte(user, i) == '\0')
        {
        strikeOutCount++;
                
        #ifdef DEBUGAUTH
        Serial.print("Failed on passcode: ");   // Debug

        Serial.print(strikeOutCount);   // Debug
        Serial.println();   // Debug
        #endif
        
        if(strikeOutCount >= 3)
        {
          strikeOutCount = 0;
          strikeOutTime = *time_t500 + 60; // 60 Seconds in the future
        #ifdef DEBUGAUTH          
            Serial.print("Strikeout until: ");  // Debug
            Serial.print(strikeOutTime);  // Debug
        #endif
        }
        
        return '\0';  // Authentication failed.
        }


      }

    } 
    //   Serial.print("Success");
   
    strikeOutCount = 0;
    return SecurityReadGroup(user); // Authentication succeeded.

  }
  else
  {
            #ifdef DEBUGAUTH
             Serial.print("Failed on strikeout");  // Debug
            #endif
    return '\0';  // Authentication failed. Strikeout time
  }

}
#endif // SECURITY


// WD WD WD WD WD      Watch Dog      WD WD WD WD WD 
// The watchdog is periodically barked at by an external CRON script
// which pokes the UNIX time into *watchDogLastBark502 and *watchDogState304 = 1 to enable
// WatchDog will snooze for  *watchDogSnooze303 seconds.
// When awaken after snooze expires, the watchDog will 'bite' and remove power for POWER_REMOVE_SECONDS
// The external CRON MUST be more frequent than the *watchDogSnooze303
// Snooze 20 minutes (1200 seconds) and CRON every 5 minutes

 #ifdef WATCHDOG

#define POWER_REMOVE_SECONDS 30
#define WATCHDOG_PIN 2

void watchDogBite()  // Remove power from modem and firewall to power cycle
{
#ifdef DEBUG
  Serial.println("Remove power");  // Debug
#endif
  digitalWrite(WATCHDOG_PIN, LOW);  // Remove power
  *watchDogLastBark502 =  *absolute_time_t501 + POWER_REMOVE_SECONDS;

  while(*watchDogLastBark502 >= *absolute_time_t501)
  {
    tickTock();   // Simply keep track of the time

  }

  digitalWrite(WATCHDOG_PIN, HIGH);  // Restore power
#ifdef DEBUG
  Serial.println("Restore power");  // Debug
#endif
  *watchDogLastBark502 = *absolute_time_t501 + *watchDogSnooze303;  // Put the dog back to sleep.
  RecordWriteEEPROM('W',POWER_REMOVE_SECONDS, *watchDogSnooze303,  *absolute_time_t501);
  *watchDogState304 = 1; // Enable if first power cycle fails.

  // Will awake again after snooze if external CRON succeeds.

}

void watchDog()  // Wake the dog?
{
  // Serial.print('W');
  if(!group && *watchDogState304 == 1)  // Only unauthenticated & set
  {
    if(*watchDogLastBark502 + *watchDogSnooze303 <= *absolute_time_t501)
    {
      watchDogBite();  // Remove power from modem and firewall to power cycle

    }
  }
  else  // Authenticated or disabled
  {
    *watchDogLastBark502 = *absolute_time_t501 + *watchDogSnooze303;  // Keep the dog asleep.

    //    Serial.print('W');
    if(*watchDogState304 == 99)  // Force a power cycle
    {
      watchDogBite(); // Force a power cycle.

    }
  }

}

 #endif
 
 

 
  // B58
 //B65

 #ifdef PING
 
 long pingDest   = 0;
 long _pings     = 0;
 long _lostPings = 0;
 unsigned long lastPing   = 0;

 long pingMs=10000;  // Default
 boolean lastLEDset=LOW;
 
 
void ping(char _dest, byte _HighLow)  //
{  
  sendMachinePacketTo('A',_dest,'R','S','2',PING_PIN,_HighLow,0,0,'0');
}


 
 void handlePings()
 {
    Serial << "mS:" << millis() - lastPing << endl;
     _pings+=1;
        if(lastLEDset)
        {
            ping(pingDest,0);
          lastLEDset = LOW;
        }
        else
        {
            ping(pingDest,1);
          lastLEDset = HIGH;
        }
        lastPing = millis();
 }
 
 
void pingEveryMSeconds(long _dest, long _Ms)
{
  pingDest = _dest;
  if(fromDev =='0')              //May only come from '0' as there is no means to stop a remote activated ping.
  {
  pingMs = _Ms;
  pingMs = constrain(pingMs,10,10000);
  
  Serial << "Ping device:" << pingDest << " every mS:" << pingMs << endl;
  
  _pings     = 0;
  _lostPings = 0;
 
  lastPing = millis();
 
  ping(_dest,0);
 
 while(!Serial.available()) // Any key sent will stop
  {
     mainTasks();
     if(lastPing+pingMs < millis() )  // Lost ping
     {
       _lostPings+=1;
       lastPing = millis();
 
       ping(_dest,0);
     }
     
  }
  Serial << " ***************  Lost Pings:" << _lostPings << " Success:" << _pings << " Percent lost: " <<  (_lostPings*100)/_pings << endl;

  }
  else
  {
    RpacketReply('^',1);  // Out of range. Can only come from '0' as there is no means to stop a remote activated ping.
  }
}

#endif  // PING



void waitUpToMsForReply(int _wait)  // Useful at startup to execute commands
{
     unsigned long _waitUntil = millis() + _wait;

     while(_waitUntil > millis() )
     {
     mainTasks();  // do stuff except serial read
     }
 
}

 
byte ASCIItoDEC1 (byte u)  // 0 to 9
{
  return constrain(u-ASCII_0,0,9);
}


int freeRam() 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void mainTasks()
{
    //  tickTock();             // Counts seconds and quarter seconds since startup Replaced with timer.event
    getAvailableSerial();   // Serial Communication Protocol

   #ifdef WIRELESS_RF24
   if (checkRF24forPayload()) // Also process received payload
   {
     #ifdef DEBRF24
     Serial << "Processed payload: " << receive_payload << endl;
     #endif
   }
   #endif

}

