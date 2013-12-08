// Built on 2.1.208 
// Should be modified for features and debuging

// Debuging
//#define DEBUG               // General


// EEPROM target.
#define ATMEGA328     // This one
//#define  ATMEGA168  // or this one. Not both.
// Be sure to pick a Tools > Board > that matches the Atmega selected above.
// Otherwise the smaller ATMEGA168
// These directives simply allow for more EEPROM records  (14 vs 56 records)
// and more code  (14k vs 30k)

#define DEBUG_EEPROM_ARRAYS   // Prints arrays at startup
#define DEBUG_EEPROM_RECORDS  // Prints records at startup

//#define DEBUGAUTH     // Authentication   requires DEBUG

//#define DEBUGREC      // Receive debug A few debug printouts use printf
//#define DEBRF24       // Wireless degugging A few debug printouts use printf
#define DEBRF24RADIO  // Use in addition to basic DEBRF24 for more details, A few debug printouts use printf

//#define DEBUG_MACHINE_PACKETS // A few debug printouts use printf

//#define DEBUG_UNIX     // UNIX time updates, A few debug printouts use printf

#define DEBUG_USING_PRINTF    // 800 bytes - A few debug printouts use printf
  #ifdef DEBUG_USING_PRINTF 
     #include "printf.h"      // Serial.print alternative
#endif                 
 
 
 // Optional feature Inclusions
    
// #define FETCH_UNIX_TIME    // Only on Controllers, When set will fetch time from bridge at startup

//#define SECURITY            // 1020 bytes - A means to authenticate user passcodes stored on EEPROM. MUST include EEPROM_RECORDS
    
//#define WATCHDOG             //  740 bytes - A means to reset if an external reset is not received periodically. Else any athentication request succeeds

#define PING                 //  990 bytes - A means to send a stream of packets to a remote device. Optimization and troubleshooting.
#define PING_PIN 6           // Set to a pin for PING with LED blink, both local and remote device. 13 is used by RF24 so may not be available

//#define TEMPERATURE          //  215 bytes - A means to read a temperature sensor.
//#define VOLTS                //   140 bytes - A means to read voltage on an analog pin

//#define ANALOG_RW            //   440 bytes - Constrained by ANALOG_PIN_MIN,ANALOG_PIN_MAX and parm2 values 0-255
                               // Set to constrain read/write pins for your board
#define  ANALOG_PIN_MIN 0
#define  ANALOG_PIN_MAX 7

#define DIGITAL_RW           //   400 bytes - Constrained by DIGITAL_PIN_MIN,DIGITAL_PIN_MAX and parm2 value 0 = LOW >0 HIGH
#define DIGITAL_PIN_MIN  2
#define DIGITAL_PIN_MAX 12

#define DHT11_TEMP_HUMD      // 575 Bytes - A means to read a DHT11 temperature and humidity sensor

// Global define



#define SECONDS_PER_MINUTE     60
#define SECONDS_PER_HOUR     3600
#define SECONDS_PER_DAY     86400
#define SECONDS_PER_WEEK   604800
#define SECONDS_PER_MONTH 2629743

#define SUNDAY      0
#define MONDAY      1
#define TUESDAY     2
#define WEDNESDAY   3
#define THURSDAYDAY 4
#define FRIDAY      5
#define SATURDAY    6



