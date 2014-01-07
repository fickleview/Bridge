// Built on 2.1.204 
// Should not be modified

#ifndef RF24setup_H
#define RF24setup_H

  
#define CE_pin 10 // Usually 10 but that is used on many shields
#define CSN_pin 9



// Prototypes

//extern RF24 radio(int,int);

extern void writePacketToPipe( char *);
extern uint8_t checkRF24forPayload();

// Global
long wirelessTimeLimit = 0xFFFFFFF;     // future UNIX time seconds to expiry, by default never.



 //B83 Machine generated packet Tracking
#define ASCII_FIRST_TRACKING_TAG 'A'     // MUST be before ASCII_LAST_TRACKING_TAG
#define  ASCII_LAST_TRACKING_TAG 'E'     // Defines last character a continious sequence tracking tags and associated records
#define TRACKING_TABLE_ENTRIES ((ASCII_LAST_TRACKING_TAG - ASCII_FIRST_TRACKING_TAG) + 1)

#define TRACKING_TAG_EXIPRY_mS 1500     // added to *absolute_time_tULmS600. Reply expected before expiry

#define  REPLY_TRACKING_TAG_CHAR 't'     // define the reply tag character used for tracking purposes. Same on all devices!
                                    
#define MACHINE_TAG '*'  // To identify untracked machine generated packets. Must be the same on all devices.
                         // 'r' reserved for untrached authenticated replies
                         // 't' reserved for tracked tags
                         // 'A' to 'Z' reserved for static reply tags used for tracking
                         

// RF24 
static char receive_payload[33]; // 32+1 to allow room for a terminating NULL char

int  MRMPRecByteCount;     // Received count pointer to MRMPSendPacketBuffer 1 to 96 bytes

static char MRMPSendPacketBuffer[97];  // Buffer --- 3 * 32 bytes RF24 packets + 1 null
static char  MRMPRecPacketBuffer[97];  // Buffer --- 3 * 32 bytes RF24 packets + 1 null

char *MRMPPacketBufferPointer = &MRMPRecPacketBuffer[0]; // Either Receive or send buffers


#define  EOP_CHAR '#'     // End Of Packet CHARacter. Must be a printable character


    #define MRMP_TYPE_INDEX 0
    #define MRMP_TO_INDEX   1
    #define MRMP_FROM_INDEX 2
    
    #define MRMP_TAG_INDEX     3
    #define MRMP_REPLY_INDEX   4
    #define MRMP_COMMAND_INDEX 5
    
    #define MRMP_PARAMAETERS_INDEX 6   
    #define MRMP_PARAM1_INDEX      8     // Reply packets only
    #define MRMP_PARAM2_INDEX     10    // Reply packets only
    #define MRMP_PARAM3_INDEX     14    // Reply packets only
    
    #define ASCII_0 48
    #define ASCII_A 65
    #define ASCII_Z 90

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

