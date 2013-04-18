// Built on 2.1.205 
// Should not be modified unlees you have more EEPROM space on your target

#ifndef EEPROM_H
#define EEPROM_H

// declarations

#define RECORD_LENGTH 9
#define RECORDS 14

#ifdef ATMEGA328
#define RECORD_FIRST      510  
#define RECORD_LAST      1014
#define RECORD_POINTER    508  
#define EEPROM_LAST_BYTE 1023

#else
#define RECORD_FIRST      385
#define RECORD_LAST       502
#define RECORD_POINTER    383
#define EEPROM_LAST_BYTE  511

#endif 

#define RECORD_FIELDS      4

// Record Offsets
#define RECORD_EVENT       0
#define RECORD_ITEM        1
#define RECORD_VALUE       3
#define RECORD_UNIX_TIME   5

int pointer              = 0;
int pointerNext          = 0;

byte msblsb              = 0;
unsigned int pointerMSBLSB = 0;


// EEPROM records
extern int RecordReadPointer();

extern void RecordWriteEEPROM(byte, unsigned int, unsigned int,  long);
extern void RecordReadEEPROM(int);
extern void RecordReadEEPROMtoString(int);

//EEPROM arrays
extern char arrayToEEPROM2nn(int, byte);
extern char arrayToEEPROM3nn(int, int);
extern char arrayToEEPROM5nn(int, long);

extern int  arrayFromEEPROM2nn(int);
extern int  arrayFromEEPROM3nn(int);
extern long arrayFromEEPROM5nn(int);

extern void arrayLoadFromEEPROM2nn(int, int);
extern void arrayLoadFromEEPROM3nn(int, int);
extern void arrayLoadFromEEPROM5nn(int, int);

#endif // EEPROM_HEADER_H
