// Built on 2.1.203 
// Should not be modified

#ifndef MMRP_MACHINE_PACKETS_H
#define MMRP_MACHINE_PACKETS_H


//B83 Machine generated packet Tracking
#define ASCII_FIRST_TRACKING_TAG 'A'     // MUST be before ASCII_LAST_TRACKING_TAG
#define  ASCII_LAST_TRACKING_TAG 'E'     // Defines last character a continious sequence tracking tags and associated records
#define TRACKING_TABLE_ENTRIES ((ASCII_LAST_TRACKING_TAG - ASCII_FIRST_TRACKING_TAG) + 1)

#define TRACKING_TAG_EXIPRY_mS 1500     // added to *absolute_time_tULmS600. Reply expected before expiry

#define REPLY_TRACKING_TAG_CHAR 't'     // define the reply tag character used for tracking purposes. Same on all devices!
                                    
#define MACHINE_TAG '*'  // To identify untracked machine generated packets. Must be the same on all devices.
                         // 'r' reserved for untrached authenticated replies
                         // 't' reserved for tracked tags
                         // 'A' to 'Z' reserved for static reply tags used for tracking
                         
// Variables and arrays
unsigned long  absolute_time_tULmSTracking[TRACKING_TABLE_ENTRIES];


// Prototypes
extern void stampTrackingTagEpires_mS(char, int);
extern boolean   trackingTagNotEpired(char)   ;
extern boolean WithinTrackingTagRange(char)   ;


extern void sendMachinePacketTo( char , char , char , char , char , int , long ,long , char )   ;



#endif // MMRP_MACHINE_PACKETS_H
