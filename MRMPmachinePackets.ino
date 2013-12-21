// Built on 2.1.203 
// Should be modified to handleTrackingTag 


/* //B83 ==========  Machine Packets and tracking facility ==================
  ==========================================================================
A facility for functions to send and track replies to mrmp machine packets.
mrmpTag byte is used to designated static tracking records. 'A' to 'Z' is usual

*/




void stampTrackingTagEpires_mS(char _tag, int _ms)
{
  if(WithinTrackingTagRange(_tag))
  {
    absolute_time_tULmSTracking[_tag - ASCII_FIRST_TRACKING_TAG] = *absolute_time_tULmS600 + _ms;
  }
} 
  
  
  
boolean trackingTagNotEpired(char _tag)
{
  if(WithinTrackingTagRange(_tag))
  {   
     #ifdef DEBUG_MACHINE_PACKETS
     Serial << "trackingTagNotEpired - Expiry time:" << absolute_time_tULmSTracking[_tag - ASCII_FIRST_TRACKING_TAG] << " Current time:" << *absolute_time_tULmS600 << endl;
     #endif
    if(absolute_time_tULmSTracking[_tag - ASCII_FIRST_TRACKING_TAG] > *absolute_time_tULmS600)
    { 
       #ifdef DEBUG_MACHINE_PACKETS
         Serial << "trackingTagNotEpired - Valid - less than" << endl;
       #endif   
         absolute_time_tULmSTracking[_tag - ASCII_FIRST_TRACKING_TAG] = 0; // to ignore all following packets  
         return true; // Valid
    }
    else
    {
         #ifdef DEBUG_MACHINE_PACKETS
        Serial << "trackingTagNotEpired - Invalid - greater than" << endl;
        #endif
           absolute_time_tULmSTracking[_tag - ASCII_FIRST_TRACKING_TAG] = 0; // to ignore all following packets
           return false; // Exired
    }
  }
  else  // not WithinTrackingTagRange
  {
     #ifdef DEBUG_MACHINE_PACKETS
     Serial << "trackingTagNotEpired - Invalid - tag out of range" << endl;
     #endif
    return false;  
   
  } 
}
 
 

boolean WithinTrackingTagRange(char _trackingTag)
{
  return ((_trackingTag >= ASCII_FIRST_TRACKING_TAG)  && (_trackingTag <= ASCII_LAST_TRACKING_TAG));
}




void sendMachinePacketTo( char _tagrmp, char _toDev, char _type, char _command, char _parameters, int _parameter1, long _parameter2,long _parameter3,long _parameter4, char _EEPROM)
{  
  
  #ifdef DEBUG_MACHINE_PACKETS
  
      if(WithinTrackingTagRange(_tagrmp))   //setup the tracking  for a reply
      {
      Serial << "Enable tracking case if necessary for:" << _tagrmp << endl;
      }    
      else
      {
        Serial << "Tracking case not defined for:" << _tagrmp << endl;
      }
  
  #endif // DEBUG_MACHINE_PACKETS
  
  stampTrackingTagEpires_mS(_tagrmp, TRACKING_TAG_EXIPRY_mS);

  tagrmp = _tagrmp;
  
  packetHeader(_command, _type,_toDev,THIS_DEV,'r'); 
  
  MRMPSendPacketBuffer[MRMP_REPLY_INDEX] = MACHINE_TAG;  // by default it is 'r'
  

   *MRMPPacketBufferPointer = _parameters;
    MRMPPacketBufferPointer++;
    
    RpacketData(_parameter1);
    
        if(_parameters >= '2')
        {
        RpacketData(_parameter2);
        
             if(_parameters >= '3')
             {
              RpacketData(_parameter3);
                if(_parameters >= '4')
               {
                RpacketData(_parameter4);
               }
             
             }
        
        }
 
      
   if(_EEPROM == 'e')
    { 
      *MRMPPacketBufferPointer = ',';
       MRMPPacketBufferPointer++;
    
      *MRMPPacketBufferPointer = _EEPROM;
       MRMPPacketBufferPointer++;
    }
    
    
    PacketFooterAndSend();
    
       #ifdef DEBUG_MACHINE_PACKETS
       Serial << "Machine packet To: " << _toDev << " MRMPSendPacketBuffer: " << MRMPSendPacketBuffer << endl;
       #endif
}

