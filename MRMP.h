// Last modified 2014-04-17 2214J
// Should be modified for features and debuging

// Debuging
//#define DEBUG               // General


// EEPROM target only one
 #define ATMEGA2560    //  4k EEPROM
// #define ATMEGA328     //  1k EEPROM
//#define  ATMEGA168    // .5k EEPROM
// Be sure to  pick a Tools > Board > that matches the Atmega selected above.
// These directives simply allow for more EEPROM records  (14, 56, 227 records)


//#define DEBUG_EEPROM
#define DEBUG_EEPROM_ARRAYS   // Prints arrays at startup
#define DEBUG_EEPROM_RECORDS  // Prints records at startup


 #define  EEPROM_RECORDS   // 900 bytes - Required -  A means to read/write EEPROM records. 



//#define DEBUGAUTH     // Authentication   requires DEBUG

//#define DEBUGREC      // Receive debug A few debug printouts
//#define DEBRF24       // Wireless degugging A few debug printouts 
//#define DEBRF24RADIO  // Use in addition to basic DEBRF24 for more details, A few debug printouts 

#define DEBUG_MACHINE_PACKETS // A few debug printouts


//#define DEBUG_UNIX     // UNIX time updates, A few debug printouts              
 
 
 // Optional feature Inclusions
    
// #define FETCH_UNIX_TIME    // Only on Controllers, When set will fetch time from bridge at startup

//#define SECURITY            // 1020 bytes - A means to authenticate user passcodes stored on EEPROM. MUST include EEPROM_RECORDS
    
//#define WATCHDOG             //  740 bytes - A means to reset if an external hardware reset is not received periodically. Else any athentication request succeeds

//#define PING                 //  990 bytes - A means to send a stream of packets to a remote device. Optimization and troubleshooting.
const int PING_PIN = 6;           // Set to a pin for PING with LED blink, both local and remote device. 13 is used by RF24 so may not be available

//#define TEMPERATURE_LM335    //  215 bytes - A means to read a temperature sensor.
//#define VOLTS                //   140 bytes - A means to read voltage on an analog pin

//#define ANALOG_RW            //   440 bytes - Constrained by ANALOG_PIN_MIN,ANALOG_PIN_MAX and parm2 values 0-255
                               // Set to constrain read/write pins for your board
const int  ANALOG_PIN_MIN =0;
const int  ANALOG_PIN_MAX =7;

//#define DIGITAL_RW           //   400 bytes - Constrained by DIGITAL_PIN_MIN,DIGITAL_PIN_MAX and parm2 value 0 = LOW >0 HIGH
const int DIGITAL_PIN_MIN  =2;
const int DIGITAL_PIN_MAX =12;

//#define DHT11_TEMP_HUMD      // 575 Bytes - A means to read a DHT11 temperature and humidity sensor


// Global define



const long SECONDS_PER_MINUTE     =60;
const long SECONDS_PER_HOUR     =3600;
const long SECONDS_PER_DAY     =86400;
const long SECONDS_PER_WEEK  = 604800;
const long SECONDS_PER_MONTH =2629743;

const int SUNDAY      =0;
const int MONDAY      =1;
const int TUESDAY     =2;
const int WEDNESDAY   =3;
const int THURSDAYDAY =4;
const int FRIDAY      =5;
const int SATURDAY    =6;



