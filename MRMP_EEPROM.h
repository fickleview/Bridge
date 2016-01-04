// Last modified 2014-09-24
// Should not be modified unlees you have more EEPROM space on your target

#ifndef EEPROM_H
#define EEPROM_H

// declarations


// RECORD_FIRST = ( EEPROM size - (records * 9))  saving first half for MRMP varabiles

const int RECORD_LENGTH = 9;


#ifdef ATMEGA2560       
const int  RECORD_FIRST=       2046; // First half of EEPROM for MRMP variables / 9 = 227 records
const int  RECORD_LAST=        4088; // last 9 byte slot that will nor overrun end of EEPROM 4095
const int  RECORD_POINTER=     2044; // RECORD_FIRST -2
const int  EEPROM_LAST_BYTE=   4095; // 4k
const int  RECORD_LAST_NOTIFY= 2042; // RECORD_FIRST -4
#endif //  // ATMEGA2560


#ifdef ATMEGA328
const int RECORD_FIRST=       510;  
const int RECORD_LAST=       1014;
const int RECORD_POINTER=     508;   
const int EEPROM_LAST_BYTE=  1023; // 1k
const int RECORD_LAST_NOTIFY= 506;
#endif // ATMEGA328

#ifdef ATMEGA128
const int RECORD_FIRST=      385;
const int RECORD_LAST=       502;
const int RECORD_POINTER=    383;
const int EEPROM_LAST_BYTE=  511;
const int RECORD_LAST_NOTIFY=381;
#endif // ATMEGA128

const int RECORD_FIELDS=      4;

// Record Offsets
const int RECORD_EVENT=       0;
const int RECORD_ITEM=        1;
const int RECORD_VALUE=       3;
const int RECORD_UNIX_TIME=   5;

int pointer              = 0;
int pointerNext          = 0;

byte msblsb              = 0;
unsigned int pointerMSBLSB = 0;


// EEPROM records
extern int RecordReadPointer();

extern void RecordWriteEEPROM(byte, unsigned int, unsigned int,  long);
extern void RecordReadEEPROM(int);
extern void RecordReadEEPROMtoString(int);
extern void RecordReadEEPROMtoNotify(int);
extern  int RecordLastEEPROMwritten();
extern  int RecordLastEEPROMwritten();
extern void RecordWriteLastNotify(int);
extern void PollRecordReadEEPROMtoNotify();
extern void incrementRecordWriteLastNotify();
extern void wipeLocalAndcloneRemoteEEPROMdataArray(char, int , int , char);
extern void readByteFromRemoteEEPROM(char, int);

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
