// Last modified 2014-04-17 2210J
// Should not be modified

#ifndef RF24setup_H
#define RF24setup_H

  
const int CE_pin =10; // Usually 10 but that is used on many other shields
const int CSN_pin =9;



// Prototypes

//extern RF24 radio(int,int);

extern void writePacketToPipe( char *);
extern uint8_t checkRF24forPayload();

// Global
long wirelessTimeLimit = 0xFFFFFFF;     // future UNIX time seconds to expiry, by default never.


// RF24 
static char receive_payload[33]; // 32+1 to allow room for a terminating NULL char

int  MRMPRecByteCount;     // Received count pointer to MRMPSendPacketBuffer 1 to 96 bytes

static char MRMPSendPacketBuffer[97];  // Buffer --- 3 * 32 bytes RF24 packets + 1 null
static char  MRMPRecPacketBuffer[97];  // Buffer --- 3 * 32 bytes RF24 packets + 1 null

char *MRMPPacketBufferPointer = &MRMPRecPacketBuffer[0]; // Either Receive or send buffers


const char  EOP_CHAR ='#';     // End Of Packet CHARacter. Must be a printable character


    const int MRMP_TYPE_INDEX =0;
    const int MRMP_TO_INDEX   =1;
    const int MRMP_FROM_INDEX =2;
    
    const int MRMP_TAG_INDEX     =3;
    const int MRMP_REPLY_INDEX   =4;
    const int MRMP_COMMAND_INDEX =5;
    
    const int MRMP_PARAMAETERS_INDEX =6;   
    const int MRMP_PARAM1_INDEX      =8;     // Reply packets only
    const int MRMP_PARAM2_INDEX     =10;    // Reply packets only
    const int MRMP_PARAM3_INDEX     =14;    // Reply packets only
    
    const int ASCII_0 =48;
    const int ASCII_A =65;
    const int ASCII_Z =90;

// To extract the character from the MRMPRecPacketBuffer
    char& MRMP_TypeInStr =        MRMPRecPacketBuffer[MRMP_TYPE_INDEX];
    char& MRMP_ToInStr =          MRMPRecPacketBuffer[MRMP_TO_INDEX];
    char& MRMP_FromInStr =        MRMPRecPacketBuffer[MRMP_FROM_INDEX];
    
    char& MRMP_TagInStr =         MRMPRecPacketBuffer[MRMP_TAG_INDEX];
    char& MRMP_ReplyInStr =       MRMPRecPacketBuffer[MRMP_REPLY_INDEX];
    char& MRMP_CommandInStr =     MRMPRecPacketBuffer[MRMP_COMMAND_INDEX];
    
    char& MRMP_ParametersInStr =  MRMPRecPacketBuffer[MRMP_PARAMAETERS_INDEX];
    
    char& MRMP_ErrorCodeInStr =  MRMPRecPacketBuffer[MRMP_PARAM1_INDEX];     // Reply packets only


 
 
char *MRMPnextBytePointer = MRMPSendPacketBuffer; // address to point to next byte in MRMP Buffer


  
  
#endif //RF24setup_H

