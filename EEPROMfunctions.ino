// Built on 2.1.205
// Should not be modified

// EEPROM feature directives
// Revised 2013-12-13 to include RECORD_LAST_NOTIFY


 #ifdef SECURITY
    #define  EEPROM_RECORDS   // 900 bytes - Required -  A means to read/write EEPROM records. 

    #include <EEPROM.h> 
  #endif


 #define  EEPROM_ARRAYS        // 1160 bytes - Optional - A means to read/write MRMP RAM arrays to EEPROM. MUST include EEPROM_RECORDS
   #ifdef EEPROM_ARRAYS    
     #define  EEPROM_RECORDS   // 900 bytes - Required -  A means to read/write EEPROM records. 
     #include <EEPROM.h>
   #endif


#define  EEPROM_RECORDS        // 1160 bytes - Optional - A means to read/write MRMP RAM arrays to EEPROM. MUST include EEPROM_RECORDS
   #ifdef EEPROM_RECORDS    
     #include <EEPROM.h>
   #endif
   
// Irrigate EEPROM functions

// The entire EEPROM


void WipeEEPROM()
{
#ifdef DEBUG_EEPROM
  Serial.println();
  Serial.println("Wiped entire EEPROM.");
#endif

  for (int i = 0; i <= EEPROM_LAST_BYTE; i++)
  {
    EEPROM.write(i,0);

#ifdef DEBUG_EEPROM
    Serial.print('0');
#endif

  }
}

// Only the forst part up to the records
void WipeEEPROMarrayOnly()
{

#ifdef DEBUG_EEPROM
  Serial.println();
  Serial.println("Wiped array EEPROM only.");
#endif

  for (int i = 0; i < RECORD_POINTER; i++)
  {
    EEPROM.write(i,0);
#ifdef DEBUG_EEPROM
    Serial.print('a');
#endif

  }
}

void WipeEEPROMrecordsOnly()
{

#ifdef DEBUG_EEPROM
  Serial.println();
  Serial.println("Wiped records EEPROM only.");
#endif

  for (int i = RECORD_POINTER; i <= EEPROM_LAST_BYTE; i++)
  {
    EEPROM.write(i,0);

#ifdef DEBUG_EEPROM
    Serial.print('r');
#endif

  }
}


// RRRRRRRRRRRRRRR EPROM Recorder 1.3 RRRRRRRRRRRRRRRRRRRRRR
// EEEEEEEEEEEEEE        Reader   EEEEEEEEEEEEEEEEEEEEEE


/*

 
 EEPROM ony permits byte read/write.
 Record packet as follows...
 Event(1), Item(2), value(2), time_t(4)   9 bytes 14 records + pointer = 127 bytes
 
 0 to 511th (1023 on ATMEGA328) byte available. We use the top quarter for records.
 Most recent record recorded (the pointer) is at 384th byte.
 This record pointer MUST be updated after successful writes pointing to the next writable record.
 
 
 EEPROM records...
 RECORD_LENGTH = 9
 RECORDS      = 14
 RECORD_FIRST = 385  ( 511 - (14 * 9)) = 385th byte )

 
 ATMEGA328 will use upper 512 bytes
 RECORD_FIRST = 510  ( 1023 - (57 * 9)) = 510th byte )
 
 RECORD_POINTER = RECORD_FIRST - 2  Contains 385, 394, 403 etc to 502
 
 ATMEGA328
 RECORD_POINTER = RECORD_FIRST - 2  Contains 510, 519, 528 etc to 1014
 RECORD_LAST_NOTIFY = RECORD_FIRST - 4  Contains last successfully notify record 
 
 RECORD_EVENT = RECORD_POINTER + RECORD_FIRST     + 0
 RECORD_ITEM = RECORD_POINTER + RECORD_FIRST      + 1
 RECORD_VALUE = RECORD_POINTER + RECORD_FIRST     + 3
 RECORD_UNIX_TIME = RECORD_POINTER + RECORD_FIRST + 5
 
 RECORD_LAST = RecordBase + (RECORDS * RECORD_LENGTH) =  502nd byte (1014)
 
 
 
 time_t is Unix time in seconds.
 RECORD_UNIX_TIMEStartup = RECORD_FIRST - 5 
 May be zero or close to it at startup if time_t is not updated at startup.
 
 
 */



 #ifdef EEPROM_RECORDS 
 
 int EEPROMReadWordAt(byte add)
 {
   byte MSByte = EEPROM.read(add);
   byte LSByte = EEPROM.read(add + 1);
   return word(MSByte,LSByte);

 }
 
int RecordReadPointer()
{
  // Get the current pointer for the next write

  // If the pointer is garbage or null at startup..
  return int(constrain(EEPROMReadWordAt(RECORD_POINTER),RECORD_FIRST,RECORD_LAST));

}


int RecordReadLastNotify()
{
  // Get the last notification sent

  return EEPROMReadWordAt(RECORD_LAST_NOTIFY);
}


// bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
void RecordWriteBYTE(int p,byte b)  // 
{
  EEPROM.write(p,b);
}



// bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb

//+++++++++++++++++++++++++++++++++++++++++++++++++++

void RecordWriteINT(int p,int i)  // Both integers
{
  msblsb = i >> 8;     
  EEPROM.write(p,msblsb);

  p++ ;

  // LSB next
  msblsb = i & B11111111; 

  EEPROM.write(p,msblsb);

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++


//LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL

void RecordWriteLONG(int p,long l)  //
{
  msblsb = l >> 24;     
  EEPROM.write(p,msblsb);

  p++ ;

  msblsb = l >> 16;     
  EEPROM.write(p,msblsb);

  p++ ;

  msblsb = l >> 8;     
  EEPROM.write(p,msblsb);

  p++ ;

  // LSB next
  msblsb = l & B11111111; 

  EEPROM.write(p,msblsb);

}



//LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL


//RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
// A facility to write four fileds at a time into EEPROM



void RecordWriteEEPROM(byte event, unsigned int item, unsigned int value,  long time)
{


  pointer =  RecordReadPointer();

  // Write the event -------------------------
  EEPROM.write((pointer + RECORD_EVENT), event);

  //    Serial.print(" recordWriteEvent:");  // Debug
  //    Serial.print(event);  // Debug

  // Write item -------------------------------
  // We need the MSB and LSB and write exch
  // MSB first



    pointerNext = pointer + RECORD_ITEM;

  RecordWriteINT(pointerNext,item);


  // Write value  -------------------------------
  // We need the MSB and LSB and write exch
  // MSB first

  pointerNext = pointer + RECORD_VALUE;

  RecordWriteINT(pointerNext,value);


  // Write time  -------------------------------
  // We need the MSB and LSB and write exch
  // MSB first

  pointerNext = pointer + RECORD_UNIX_TIME;
  RecordWriteLONG(pointerNext,time);




  // Update the pointer
  pointer = pointer + RECORD_LENGTH; 

  if (pointer > RECORD_LAST)
  {
    pointer = RECORD_FIRST;
  }
  //  Serial.println();
  //  Serial.print(" recordWritePointerNext:");  // Debug
  //   Serial.print(pointer);  // Debug

  RecordWriteINT(RECORD_POINTER,pointer);

}

//RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR

long value;
long valueNext;


byte RecordReadEPROMbyte(int p)
{
  return EEPROM.read(p);
}


int RecordReadEPROMint(int p)
{
  msblsb = EEPROM.read(p);

  p++;
  value = msblsb;
  value = value << 8;

  // LSB next
  msblsb = EEPROM.read(p);

  value = value + msblsb;
  return value;

}

long RecordReadEPROMlong(int p)
{
  value = 0;


  msblsb = EEPROM.read(p);

  p++;
  valueNext = msblsb;
  value = value + (valueNext << 24);

  msblsb = EEPROM.read(p);

  p++;
  valueNext = msblsb;
  value = value + (valueNext << 16);

  msblsb = EEPROM.read(p);

  p++;
  valueNext = msblsb;
  value = value + (valueNext << 8);

  // LSB next
  msblsb = EEPROM.read(p);


  return value + msblsb;

}


// Prints the selected record

#ifdef DEBUG_EEPROM_RECORDS
void RecordReadEEPROM(int record)
{

  record = RECORD_FIRST + (record * RECORD_LENGTH);
  if(record > RECORD_LAST)
  {
    record = RECORD_FIRST; // Rollover
  }
  record = constrain(record,RECORD_FIRST,RECORD_LAST);


  // Record  ============================
  msblsb = EEPROM.read(record);

  value = msblsb;

  Serial.println();
  Serial.print(EOP_CHAR);  // Debug
  Serial.print(record);
  Serial.print(',');
  Serial.print(value);  // Debug


  // Record Item ============================
  //MSB

  value = RecordReadEPROMint(record + RECORD_ITEM);


  Serial.print(" I:");  // Debug
  Serial.print(value);  // Debug

  // Record Value ============================


  value = RecordReadEPROMint(record + RECORD_VALUE);


  Serial.print(" V:");  // Debug
  Serial.print(value);  // Debug


  // Record Time ============================

  //MSB

  value = RecordReadEPROMlong(record + RECORD_UNIX_TIME);

  Serial.print(" T:");  // Debug
  Serial.print(value);  // Debug
}
#endif // EEPROM_RECORDS 


#endif // DEBUG

#ifdef EEPROM_RECORDS

// Sends four reply fields per record (byte, unsigned int, unsigned int,  long)
//B66
void RecordReadEEPROMtoString(int _record)
{

  char _reply ='r';
  if(WithinTrackingTagRange(tagrmp))
  {
    _reply = REPLY_TRACKING_TAG_CHAR;
  }

  packetHeader('E', 'R',fromDev,toDev,_reply);
 
    *MRMPPacketBufferPointer = '7';  
    MRMPPacketBufferPointer++;

   *MRMPPacketBufferPointer = ',';
    MRMPPacketBufferPointer++;
    
    *MRMPPacketBufferPointer = '0';
    MRMPPacketBufferPointer++;
    
    
    RpacketData(parm1);  
    RpacketData(_record);

    RecordReadEEPROMtoStringFooter(_record);
  
}

// Notifications
void RecordReadEEPROMtoNotify(int _record)
{
    // Change record number to EEPROM address
  _record = RECORD_FIRST + (_record * RECORD_LENGTH);
  if(_record > RECORD_LAST)
  {
    _record = RECORD_FIRST; // Rollover
  }
  _record = constrain(_record,RECORD_FIRST,RECORD_LAST);

int  _parm1 = EEPROM.read(_record);
int  _parm2 = RecordReadEPROMint(_record + RECORD_ITEM);
int  _parm3 = RecordReadEPROMint(_record + RECORD_VALUE);
long _parm4 = RecordReadEPROMlong(_record + RECORD_UNIX_TIME);

 sendMachinePacketTo('B', NOTIFY_DEV, 'R', 'N', '4', _parm1 , _parm2, _parm3, _parm4, '0');  // 'B' is tracked and reply recorded
 

}



void RecordReadEEPROMtoStringFooter(int _record)
{
  // Change record number to EEPROM address
  _record = RECORD_FIRST + (_record * RECORD_LENGTH);
  if(_record > RECORD_LAST)
  {
    _record = RECORD_FIRST; // Rollover
  }
  _record = constrain(_record,RECORD_FIRST,RECORD_LAST);

  // Record type/notify level ============================
  RpacketData(EEPROM.read(_record));


  // Record Item ============================
  //MSB
  RpacketData(RecordReadEPROMint(_record + RECORD_ITEM));


  // Record Value ============================
  RpacketData(RecordReadEPROMint(_record + RECORD_VALUE));

  // Record Time ============================

  //MSB
  RpacketData(RecordReadEPROMlong(_record + RECORD_UNIX_TIME));

  PacketFooterAndSend();

}



int RecordLastEEPROMwritten()  // starting with 0 
{
     int p;
              p  = RecordReadPointer();  // Points to the next record to write.
    
              p = p - RECORD_LENGTH;  // Go back one record.
                  if (p < RECORD_FIRST)  // Is it wrapping?
                  {
                    p = RECORD_LAST; 
                  }
              p = (p - RECORD_FIRST) / RECORD_LENGTH;
              return p;
}

void RecordWriteLastNotify(int _p)
{
  
   RecordWriteINT(RECORD_LAST_NOTIFY,_p);
  
}

int RecordReadLastNotifyNext(int _p) // starting with 0 handles wrapping
{
        _p = _p + RecordReadLastNotify();
        _p = (_p * RECORD_LENGTH) + RECORD_FIRST;

    
                  if (_p > RECORD_LAST)  // Is it wrapping?
                  {
                    _p = RECORD_FIRST; 
                  }
                  
              _p = (_p - RECORD_FIRST) / RECORD_LENGTH;
              return _p;
}

  // check re-send records that have not been successfully sent 
  void PollRecordReadEEPROMtoNotify()
  {
    #ifdef DEBUG_EEPROM_RECORDS

          Serial << endl << endl << "RecordLastEEPROMwritten: " << RecordLastEEPROMwritten() << " RecordReadLastNotify: " << RecordReadLastNotify() << endl;
          Serial << "NOTIFY_DEV: " << NOTIFY_DEV << " THIS_DEV: " << THIS_DEV << endl << endl;

    #endif //  DEBUG_EEPROM_RECORDS

    if((RecordLastEEPROMwritten() != RecordReadLastNotify()) && (NOTIFY_DEV != THIS_DEV)) // Records not sent & not to self.
    {     
      RecordReadEEPROMtoNotify(RecordReadLastNotifyNext(1));           
    }
    
  }
  
 void incrementRecordWriteLastNotify() // Records are stored starting with 0
  {
  RecordWriteLastNotify(RecordReadLastNotifyNext(1)); // starting with 0 handles wrapping
  }
  
#endif //EEPROM_RECORDS


// VVVVVVVVVVVVVVVVV arrayToEEPROM VVVVVVVVVVVVVVVVVVVV

// This function is designed to read and write one, two and four byte varables to EEPROM
// for restore of RAM arrays on Setup.

#define ARRAY_TO_EEPROM_BASE 0   // uses the very first record in EEPROM
#define  ARRAY_TO_EEPROM_MAX  RECORD_POINTER - 20  // Bytes reserved for saving. MUST not collide with other EEPROM

// Following two stubbed off for debug and to save RAM

/*
char arrayToEEPROM3nn(int i, int v)
 {
 return '^';  // Out of range
 }
 
 char arrayToEEPROM5nn(int i, long v)   // Saves 500 series long specified after integers
 {
 return '^';  // Out of range
 }
 
 */

#ifdef EEPROM_ARRAYS

int EEPROMadd5xx(int i)
{
  return ARRAY_TO_EEPROM_BASE + BYTE_FIELD_SIZE + (2 * INT_FIELD_SIZE) + (i * 4);
}


int EEPROMadd3xx(int i)
{
  return ARRAY_TO_EEPROM_BASE + BYTE_FIELD_SIZE + (2 * i);
}

int EEPROMadd2xx(int i)
{
  return ARRAY_TO_EEPROM_BASE + i;
}



char arrayToEEPROM2nn(int i, byte v)   // Saves 200 series integer specified
// i 0 to 99

{
  if(i > BYTE_FIELD_SIZE  || EEPROMadd2xx(i) > ARRAY_TO_EEPROM_MAX )  
  {
    return '^';  // Out of range
  }
  else
  {
    RecordWriteBYTE(EEPROMadd2xx(i), v);
    return '0';  // No error.

  }
}


char arrayToEEPROM3nn(int i, int v)   // Saves 300 series integer specified
// i 0 to 99

{
  if(i > INT_FIELD_SIZE  || EEPROMadd3xx(i) > ARRAY_TO_EEPROM_MAX )  
  {
    return '^';  // Out of range
  }
  else
  {
    RecordWriteINT(EEPROMadd3xx(i), v);
    return '0';  // No error.
  }
}


char arrayToEEPROM5nn(int i, long v)   // Saves 500 series long specified after integers
// i 0 to 99

{

  if(i >= LONG_FIELD_SIZE || EEPROMadd5xx(i) > ARRAY_TO_EEPROM_MAX - 4 )  // Four bytes for a long
  {
    return '+';  // Out of range long
  }
  else
  {
    RecordWriteLONG( EEPROMadd5xx(i), v);
    return '0';  // No error.

  }
}

int arrayFromEEPROM2nn(int i)
{
  return RecordReadEPROMbyte(EEPROMadd2xx(i));

}

int arrayFromEEPROM3nn(int i)
{
  return RecordReadEPROMint(EEPROMadd3xx(i));

}

long arrayFromEEPROM5nn(int i)
{
  return RecordReadEPROMlong(EEPROMadd5xx(i));

}

// Use these at startup to load 200, 300 and 500 range varable from EEPROM

void arrayLoadFromEEPROM2nn(int f, int l)  // First and Last 200 range to load
{
  if(l <= BYTE_FIELD_SIZE)  // Do not over write
  {
    for (int i=f; i <= l; ++i) 
    {
      *(byteField + i)  = arrayFromEEPROM2nn(i);
    }
  }
}

void arrayLoadFromEEPROM3nn(int f, int l)  // First and Last 300 range to load
{
  if(l <= INT_FIELD_SIZE)  // Do not over write
  {
    for (int i=f; i <= l; ++i) 
    {
      *(intField + i)  = arrayFromEEPROM3nn(i);
    }
  }
}

void arrayLoadFromEEPROM5nn(int f, int l)  // First and Last 500 range to load
{
  if(l <= LONG_FIELD_SIZE)  // Do not over write
  {
    for (int i=f; i <= l; ++i)  
    {
      *(longField + i)  = arrayFromEEPROM5nn(i);
    }
  }
}

#endif // EEPROM_ARRAYS

