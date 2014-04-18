// Should not be modified

#ifndef kMRMPfunctions_h
#define kMRMPfunctions_h

  
//Prototypes

extern void RpacketData(long);
extern void ByePacketFrom(char, char)  ;
extern void packetHeader(char, char, char, char, char)  ;

extern void PacketFooterAndSend()  ;
extern void RpacketReply( char, long)  ;
extern void okay();


extern   void getData();
extern   void putData();
extern   void RpassCodeReply(int);

extern   void getUserPassCode();
extern   void putUserPassCode();
extern   void parseRecHeader();

extern   void executeCommand();
extern   void getAvailableSerial();
extern   char elementInRouteTable(char);

extern   void parseRecPacket();

extern   int SecurityValidatePass(int);
extern   void watchDog();


extern   void mainTasks();
extern   int freeRam();

extern   byte ASCIItoDEC1 (byte);


//MRMP Globals
char fromDev    = '0';       // The 'control' device by default 0 to 9 valid
char tagrmp     = '?';       // tag for packet ASCII 33 to 127  'A' to 'Z' reserved for tracking
char packetType = '0';       // I K R B G P 0
char command    = '0';       // Byte 4 These following by parameters and values

char group      = '\0';      // group and user for authenticated access
byte user       = 0;

char reply      = 'n';       // 5th char is a flgg. 'r' is a reply packet. REPLY_TRACKING_TAG_CHAR tracking packet

char processing = 'N';
long lastProcessTime;         // Used to timeout processing of packest from dead senders


byte commandParameters = 0;  //Byte 5 


char *parameterPointerArray[10];   // MRMP Parameter array

// Bytes 7 to 9. See field array notes. [1]
int field     = 0;           // 00 to 99  bytes 8 aand 9
int fieldType = 0;           // 100, 200, 300 to 900 byte 7 + 00

// Followed by up to 9 comma seperated data elements
#define MAX_PARAMETERS 9   //  1 2 3 = number of the following string Data arrays.

long parm1;               // First  MRMP Parameter converted long
long parm2;               // Second MRMP Parameter converted long
long parm3;               // Third  MRMP Parameter converted long
long parm4;               // Fourth MRMP Parameter converted long

char errorMRMP = '?';
char inByte = 0;          // Byte read from serial port

char  toDev    = '?';     // Device code any printable ASCII valid


char EEPROMPrm = '*';     // 2nd or 3rd parameter for EEROM r/w

                         
#endif // kMRMPfunctions_h
