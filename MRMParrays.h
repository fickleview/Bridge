// Last modified 
// Modify for your sketch

#ifndef MRMParrays_H
#define MRMParrays_H

  
// ==================== MRMP Array Get and Put =========================
// Field arrays are used for data that will be either Put (write) or Get (read)
// The objective is to Get or Put by 'index' being the last two digits of the field

// 503 Represents 500 range for long and the 03 third index of longField array
// Example: 503 is LONG_FIELD_BASE + field 
//          503  = 500             + 3

// 500 is used by P or G to switch to the longField array
// 3 is used to put or get the 3rd element of longField array

// 000 to 099 Arbitrary data

// xx_FIELD_SIZE must be at least one more that the number of defined pointers, as the first is zero.
// Increasing xx_FIELD_SIZE beyong what is need uses more RAM
// Changing xx_FIELD_SIZE after EEPROM arrays are written will cause misalignment of read elements from EEPROM
// xx_FIELD_SIZE is used to capture reads/writes beyond the end of the defined array.

// char          elements 100 to 199
const int CHAR_FIELD_BASE =100;
//#define CHAR_FIELD_SIZE   2    
char charField[CHAR_FIELD_SIZE];

// byte          elements 200 to 299
const int BYTE_FIELD_BASE =200;
//#define BYTE_FIELD_SIZE   3
byte byteField[BYTE_FIELD_SIZE];

byte *Address200 = byteField;         // Used by MRMP as ASSCII MRMP address.
byte *DebugLevel201 = byteField+1;    // Verbosity shown in Okay packet reply,  0 : quiet, 1: basic, 2: verbose 3: extreme




// int           elements 300 to 399
const int INT_FIELD_BASE =300;
//#define INT_FIELD_SIZE  11
int intField[INT_FIELD_SIZE];

// The following three are reserved fields across all controlers. Allows Bridge to maintain a source for updates.
int *TZ300      = intField;     // R01**P3,300,-3,e# Hours Local time zone incluting the DST adjust +12 to -12 hours
int *WeekDay301 = intField +1;  // 0 = Sunday
int *Date302    = intField +2;  // Calendar date


int *watchDogSnooze303 = intField + 3;  // Seconds the watchdog will snooze before it wakes
int *watchDogState304  = intField + 4;  // A flag to force a power cycle.

int  *insideTemp305    = intField +5;   //
int *outsideTemp306    = intField +6;   //

// unsigned int  elements 400 to 499
const int UINT_FIELD_BASE =400;
//#define UINT_FIELD_SIZE   0
unsigned int UintField[UINT_FIELD_SIZE];

// long          elements 500 to 599
const int LONG_FIELD_BASE =500;
//#define LONG_FIELD_SIZE   4
long longField[LONG_FIELD_SIZE];
// 0 is UNIX time
long *time_t500           = longField + 0;  // Seconds since startup, never changed
long *absolute_time_t501  = longField + 1;  // Updated periodically

long *watchDogLastBark502 = longField + 2;  // UNIX time of last successful ping


// unsigned long elements 600 to 699
const int ULONG_FIELD_BASE =600;
//#define U_LONG_FIELD_SIZE  2
unsigned long UlongField[U_LONG_FIELD_SIZE];
unsigned long *absolute_time_tULmS600 = UlongField    ;  // absolute_time_tUL601 plus mS - Updated periodically
unsigned long *absolute_time_tUL601   = UlongField + 1;  // UL Updated periodically. Same as absolute_time_t501 but will not rollover

// float         elements 700 to 799
const int FLOAT_FIELD_BASE =700;
const int FLOAT_FIELD_SIZE   =0;
float floatField[FLOAT_FIELD_SIZE];

//http://www.nongnu.org/avr-libc/user-manual/group__avr__stdlib.html#ga6c140bdd3b9bd740a1490137317caa44
// dtostrf  dtostre
// double        elements 800 to 899
const int DOUBLE_FIELD_BASE =800;
//#const int DOUBLE_FIELD_SIZE  = 0;
double doubleField[DOUBLE_FIELD_SIZE];

// others        elements 900 to 999  such as temp snsors
// 900 series is different in that it fetches data from a port
const int OTHER_BASE =900;
const int OTHER_BASETemp =90;  // 990 to 999 channels 0 to 9
const int OTHER_BASEAuth =80;  // 880 to 889 users 0 to 9 ( 1 to 8 valid UID)

#endif // #define MRMParrays_H
