// Last modified 
// Modify for your sketch


  
//RF24 Routing table
/*
Writing pipes for packet Routing
Bridge can be any character, but by default is UPPER case 'X'  'Y' etc
Controllers typically lower case letters

Port '0' is by default the local serial port.
 '*' in the tables, is an undefined port, either local or remote. 
 Undefined ports will print to the local serial port.
 Only one numbered port in entire network is recommended.

*/
  
// Four sample configurations
// DO NOT forget to define a valid directive above BRIDGE_ or CONTROLLER_ defined in these samples


//============= YOU MUST CHOOSE THESE DIRECTIVES and serial port/board in Arduino===================
// ================================================================================================

// 64 bit RF24 pipe static transmit and receive addresses. All unique.
 
const  uint64_t RF24_TX_RX_PIPE_ADD_X =0x0101010101LL;  // X

const  uint64_t RF24_TX_RX_PIPE_ADD_A =0x010101010aLL;  // a
const  uint64_t RF24_TX_RX_PIPE_ADD_B =0x010101010bLL;  // b
const  uint64_t RF24_TX_RX_PIPE_ADD_C =0x010101010cLL;  // c
const  uint64_t RF24_TX_RX_PIPE_ADD_D =0x010101010dLL;  // d


// **************** Sample congigurations 1 *********************
//       ***********************************************

const char NOTIFY_DEV      ='X';      // where to send notifications
const char UNIXTIME_ON_DEV ='X';      // where to fetch the UNIX time at startup
                                 // the UNIX time is updated on UNIXTIME_ON_DEV by a script (perl) on the bridged computer

  // Topology sample 1 'X' is the hub for all devices. 
  // However, it is perfectly okay for a to communicate with b directly if you desire
  //    Y <SOFT_SERIAL_PIPE> X <RF24_TX_RX_PIPE_ADD_A> a
  //                         X <RF24_TX_RX_PIPE_ADD_B> b
 
 


// Begin config 'X' sample 1  ===========================


// Bridge device X is both RF24 wireless and SoftSerialcapable
  //    Y <SOFT_SERIAL_PIPE> X <RF24_TX_RX_PIPE_ADD_A> a
  //                         X <RF24_TX_RX_PIPE_ADD_B> b

const char THIS_DEV ='X';
const int kNumberOfPipes =5;
   const  uint64_t pipes[] = {RF24_TX_RX_PIPE_ADD_X,RF24_TX_RX_PIPE_ADD_A,RF24_TX_RX_PIPE_ADD_B,RF24_TX_RX_PIPE_ADD_C,RF24_TX_RX_PIPE_ADD_D};   // Transmit to RF24 address port list. X[0] a[1]  b[2] c[3] d[4]
   const char routeTable[] = "a1,b2,c3,d4,0*,Y',1*,2',3*,4*,5*,6*,7*,8*,9*";     // Comma delimited pairs. toDev and Port ie write to a use port 0
   
  #define WIRELESS_RF24        // Required to compile RF24 wireless library and functions

//
#define SOFTSERIAL_BRIDGING                                                 // - A means to brige two devices via SoftSerial
  #define SOFT_SERIAL_PIPE    SOFT_SERIAL_PIPE_Y  // ASCII ' in route table
  
   #define SOFT_SERIAL_PIPE_Y 39  // ASCII ' in route table 
   #define SOFT_SERIAL_PIPE_X 38  // ASCII & in route table


  
  // End config 'X' sample 1  ===========================

  
  

// Begin config 'a' sample 1  ===========================

/*
// Device 'a' is RF24 wireless only and talks to X but not 'b' directly (but it could if in range)
 //    Y <SOFT_SERIAL_PIPE> X <RF24_TX_RX_PIPE_ADD_A,RF24_TX_RX_PIPE_ADD_A> a
 //                         X <RF24_TX_RX_PIPE_ADD_B> b'
  
    #define THIS_DEV 'a'
    #define kNumberOfPipes 1
   const  uint64_t pipes[] = {RF24_TX_RX_PIPE_ADD_A};                        // Transmit to RF24 address port list
   const char routeTable[] = "b0,c0,d0,X0,Y0,0*,10,20,3*,4*,5*,6*,7*,8*,9*"; // Do Not include THIS_DEV.  Comma delimited pairs. toDev and Port

  #define WIRELESS_RF24        // Required to compile RF24 wireless library and functions
 //B92 now using kNumberOfPipes //   #define RECEIVE_ON_RF24_ADD    RF24_TX_RX_PIPE_ADD_A  
  #define FETCH_UNIX_TIME

// End config 'a' sample 1  ===========================
*/


/*
// Begin config 'b' sample 1  ===========================

// Device 'b' is RF24 wireless only and talks to X but not 'a' directly (but it could if in range)
 //    Y <SOFT_SERIAL_PIPE> X <RF24_TX_RX_PIPE_ADD_A> a
 //                         X <RF24_TX_RX_PIPE_ADD_B> b'
 
 
  #define THIS_DEV 'b'
  #define kNumberOfPipes 1
   const  uint64_t pipes[] = {RF24_TX_RX_PIPE_ADD_B};                          // Transmit to RF24 address port list
   const char routeTable[] = "a0,c0,d0,X0,Y0,0*,10,20,3*,4*,5*,6*,7*,8*,9*";   // Do Not include THIS_DEV.  Comma delimited pairs. toDev and Port

  #define WIRELESS_RF24        // Required to compile RF24 wireless library and functions
//B92 now using kNumberOfPipes //    #define RECEIVE_ON_RF24_ADD    RF24_TX_RX_PIPE_ADD_B  
  #define FETCH_UNIX_TIME
// End config 'b' sample 1  ===========================
*/



/*
// Begin config 'c' sample 1  ===========================

// Device 'b' is RF24 wireless only and talks to X but not 'a' directly (but it could if in range)
 //    Y <SOFT_SERIAL_PIPE> X <RF24_TX_RX_PIPE_ADD_A> a
 //                         X <RF24_TX_RX_PIPE_ADD_B> b'
 
 #define THIS_DEV 'c'
 #define WIRELESS_RF24        // Required to compile RF24 wireless library and functions

  #define kNumberOfPipes 1
   const  uint64_t pipes[] = {RF24_TX_RX_PIPE_ADD_C};                          // Transmit to RF24 address port list
   const char routeTable[] = "a0,b0,d0,X0,Y0,0*,10,20,3*,4*,5*,6*,7*,8*,9*";   // Do Not include THIS_DEV.  Comma delimited pairs. toDev and Port


 #define FETCH_UNIX_TIME
// End config 'c' sample 1  ===========================
*/

/*
// Begin config 'd' sample 1  ===========================

 #define THIS_DEV 'd'
 #define WIRELESS_RF24        // Required to compile RF24 wireless library and functions

  #define kNumberOfPipes 1
   const  uint64_t pipes[] = {RF24_TX_RX_PIPE_ADD_D};                          // Transmit to RF24 address port list
   const char routeTable[] = "a0,b0,c0,X0,Y0,0*,10,20,3*,4*,5*,6*,7*,8*,9*";   // Do Not include THIS_DEV.  Comma delimited pairs. toDev and Port

 #define FETCH_UNIX_TIME
 
// End config 'c' sample 1  ===========================
*/

/*

// Begin config 'Y' sample 1  ===========================


// Device 'Y' is softSerial only and talks to X but not a or b directly
 //    Y <SOFT_SERIAL_PIPE> X <RF24_TX_RX_PIPE_ADD_A> a
 //                         X <RF24_TX_RX_PIPE_ADD_B> b'
 
 // SofSerial pipe ASCII address

 //#define SOFT_SERIAL_PIPE_Y 39  // ASCII ' in route table 
 //#define SOFT_SERIAL_PIPE_X 38  // ASCII & in route table

  #define THIS_DEV 'Y'
 
   const char routeTable[] = "a&,b&,c&,X&,0*,1*,2&,3*,4*,5*,6*,7*,8*,9*";      // Do Not include THIS_DEV.  Comma delimited pairs. toDev and Port


  #define SOFTSERIAL_BRIDGING                                                  // - A means to brige two devices via SoftSerial
  #define SOFT_SERIAL_PIPE    SOFT_SERIAL_PIPE_X // ASCII & in route table
  #define FETCH_UNIX_TIME
// End config 'Y' sample 1  ===========================

// End of Sample 1
*/



