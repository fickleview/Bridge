// Built on 2.1.209 
// Should not be modified
// RF24 



#ifdef WIRELESS_RF24
   #include <SPI.h>
   #include <nRF24L01.h>
   #include <RF24.h>

   
#endif //WIRELESS_RF24




#ifdef WIRELESS_RF24
// WWWWWWWWWWWWWWWWWWW  Wireless WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
// WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
// 

//  RF24 RF24 RF24  RF24 RF24 RF24  RF24 RF24 RF24  RF24 RF24 RF24  RF24 RF24 RF24  RF24 RF24 RF24 
//  RF24 RF24 RF24  RF24 RF24 RF24  RF24 RF24 RF24  RF24 RF24 RF24  RF24 RF24 RF24  RF24 RF24 RF24 

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus variable CE_pin & CSN_pin
// Interrupt is not used.
/*
NRF > Arduino
1 > GND
2 > 3v3
3 > 9  CSN = CSN_pin
4 > 10 CE  = CE_pin
5 > 13 SCK
6 > 11 MOSI
7 > 12 MISO
8 > 8  IRQ  // Not used 

*/

   RF24 radio(CE_pin,CSN_pin);


//
//
// Payloads
//
//static char receive_payload[33]; // 32+1 to allow room for a terminating NULL char

//static int MRMPSendByteCount;  // 1 to 96 bytes
//static char MRMPSendPacketBuffer[97];  // 3 * 32 RF24 packets + 1 null
//static  long *MRMPnextBytePointer; // address to point to next chunk on MRMPSendPacketBuffer




void writeRF24payload(uint8_t _pipe, char *payload, uint8_t bytes)
{
   radio.stopListening();

       // Open the correct pipe for writing

    radio.openWritingPipe(pipes[_pipe]);
    
#ifdef DEBRF24
        long _add = pipes[_pipe];
        Serial << F(">> Opened pipe#: ") <<  _HEX(_pipe) << F(" Add: ") << _HEX(_add) << endl;
       // radio.printDetails();
#endif // DEBRF24
        
  delay(5); // necessary when not printing details

      


    // Take the time, and send it.  This will block until complete
 
boolean writeSuccess =false;

         writeSuccess =  radio.write( payload, bytes );
         
#ifdef DEBRF24
Serial << F("Write returned:") << writeSuccess << endl;
#endif // DEBRF24

 
       // Now, continue listening
   

   radio.startListening();



}
#endif

//B77
uint8_t pipe_num = 0;  // '0'=0 Always. Set by routing table on send

void writePacketToPipe( char *_buffer)  // packet buffer can be mutiples of 32 max bytes allowed by RF42, so send in chunks
{
  

  int  _count =   strlen(_buffer);
  
  char addr =      *(_buffer+ MRMP_TO_INDEX);
  char _reply = *(_buffer+ MRMP_REPLY_INDEX);

  
  
  uint8_t len;
  
    pipe_num = 'N';
    pipe_num = elementInRouteTable(addr); // scans routing table for a pipe to send on
    
       #ifdef DEBUGREC
       Serial << F("Pipe ASCII:") << pipe_num << F(" Write Count:") << _count << F(" To:") << addr << F(" Writing:") <<  _buffer << endl;
       #endif
         
         
  if(pipe_num != 'N' &&  !(pipe_num <= 47)) // // 'N' Not found    hosted port between ASCII ' and /
    {
    #ifdef WIRELESS_RF24
     
      stampTrackingTagEpires_mS(*(_buffer+ MRMP_TAG_INDEX),TRACKING_TAG_EXIPRY_mS);
     
      pipe_num = pipe_num-ASCII_0;
         
      for (int chunks = (_count/32); chunks >= 0; chunks--)
      {
    
        len = 32; // the largest we can send
        MRMPnextBytePointer = _buffer +((_count/32)-chunks)*32;
    
      #ifdef DEBRF24
       Serial << F("#> Pipe int:") << pipe_num << F(" Chunks:") << chunks <<  " Length:" << len <<  " Current chunk:" << MRMPnextBytePointer-_buffer << endl;
       #endif
    
    
    // B58
        if(!chunks)  // last or only chunk 0
        { 
          len = _count-(MRMPnextBytePointer - _buffer);  // Count less the Difference in two buffer address pointers 
        }
    
  
       #ifdef DEBRF24
       Serial << F("  *>> ") << len << F(" >") << MRMPnextBytePointer << endl;
       
       #endif
       
        writeRF24payload(pipe_num,MRMPnextBytePointer,len+1);
      } 
      
      #else
             Serial << F("WIRELESS_RF24 undefined") << endl;
      #endif
    }
    else
    {
        //B79
        //B83
          #ifdef SOFTSERIAL_BRIDGING
        if(pipe_num == SOFT_SERIAL_PIPE)  //  ASCII
        {
           stampTrackingTagEpires_mS(*(_buffer+ MRMP_TAG_INDEX),TRACKING_TAG_EXIPRY_mS);
           
             #ifdef SERIAL_BRIDGE_1_DEBUG
             Serial << F("Sent to SerialBridge1: ") << _buffer << endl;
             #endif
          SerialBridge1.print(_buffer);

        }
         #else  // SOFTSERIAL_BRIDGING
        if(0)
          {
          }
         
         #endif // SOFTSERIAL_BRIDGING
         
        else
        {
        //B83 Local pipe 
           if( _reply == REPLY_TRACKING_TAG_CHAR)
           {
               if(*DebugLevel201 > 2)
              {
               Serial  <<  MRMPRecPacketBuffer << endl;
              }
              //B84
             #ifdef DEBUG_MACHINE_PACKETS
              Serial  <<  "Handle tracking tag: " << MRMPRecPacketBuffer << endl;
             #endif
              
               handleTrackingTag();
              
           }
           
           if (_reply == 'r')
           {
             Serial  << _buffer << endl;
           }
        }
        
 
                  
                  
    }
}

  #ifdef WIRELESS_RF24
 
  
uint8_t checkRF24forPayload()
{

  
   if ((processing == 'N') || (processing == 'R'))
  {

     if ( radio.available())    
     {
      uint8_t len;
      bool done = false;
      
      	len = radio.getDynamicPayloadSize();
	done = radio.read( receive_payload, len );

        
	// Put a zero at the end for strcpy
	receive_payload[len] = 0;

#ifdef DEBRF24
  Serial << F("#< Received:") << receive_payload << endl;
#endif
       
        processing = 'R';
       
         if(MRMPRecByteCount+len <96)
         {
         //B60  Fill the receive buffer 
            strcpy (MRMPRecPacketBuffer+MRMPRecByteCount,receive_payload);
        
            MRMPRecByteCount=MRMPRecByteCount+len;
            
             #ifdef DEBUGREC
             Serial << F("  MRMPRecByteCount: ") << MRMPRecByteCount << F(" End: ") << MRMPRecPacketBuffer[strlen(MRMPRecPacketBuffer)-1] << F(" MRMPRecPacketBuffer: ") << MRMPRecPacketBuffer << endl;
             #endif
       
       
            if((MRMPRecPacketBuffer[strlen(MRMPRecPacketBuffer)-1]) == EOP_CHAR)
             {
                #ifdef DEBUGREC
                Serial << F("  Complete packet: ") << MRMPRecPacketBuffer << endl;
                #endif
             
              MRMPRecByteCount=0;
                   
                 parseRecPacket();
                 processing = 'N'; // All chunks have been processed
            
              
             }
             else
             {
            
             #ifdef DEBUGREC
             Serial << F("  Incomplete packet: ") << MRMPRecPacketBuffer << endl;
             #endif
             }
             
         }
         else   // Will not fit so...
         {
            RpacketReply('>',MRMPRecByteCount);
            MRMPRecByteCount=0;
            processing = 'N'; // All chunks have been processed
           
            
             #ifdef DEBUGREC
             Serial << F("  RF24 Packet overrflow") << endl;
             #endif
            
         }
         
      return done;  
    }
    else
    {
       return false; // no payload
    }
    
  }
  else
  {
  return false; // busy
  } 
  
}   

#endif



