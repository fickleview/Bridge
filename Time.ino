// Built on 2.1.204 
// Should not be modified 

// TTTTTTTTTTTTTTTT  Time 1.1 *****************************
// tttttttttttttttttttttttttttttttttttttttttttttttttttt
// To do - hardware clock
// Counts seconds and quarter seconds since startup.
// 86400 seconds per day is UNIX Time time_t
// 000 is 3 * 60 * 60 = 10800 + 86400 = 92700
// 3600 per minute


byte secondBoundary = false;

unsigned long cS_ticks;                  // 100 ms increments since startup.
unsigned long PriorcS_ticks;             // Used for rollover detection

int mS_time_t()   // 0 to 999 ms between seconds
{
  return millis() % 1000;
}

// *TZ300                    // Hours Local time zone incluting the DST adjust +12 to -12 hours

byte hourNow(long hrsoffset)  // Returns the current hour adjusted by hours offset. Time zone rule etc.
{
  return max(0,((*absolute_time_t501 + ( hrsoffset * SECONDS_PER_HOUR)) % SECONDS_PER_DAY) / SECONDS_PER_HOUR); 
  // Could be negative if unix time time not set during first hours of TZ300 offset.

}

byte minuteNow(long minsoffset) // Returns the current minute adjusted by minutes offset.  NFLD is +30 for example
{
  return max(0,(*absolute_time_t501 + (minsoffset * SECONDS_PER_MINUTE)) /SECONDS_PER_MINUTE);
}

long midnightUnixTimeWas()
{
  return max(0,*absolute_time_t501 - (*absolute_time_t501 + (*TZ300 * SECONDS_PER_HOUR)) % SECONDS_PER_DAY );
}

/*  Use hourIs
int hourToday()  // Returns the current hour adjusted for TZ
{
  int h;
  h = int(((((*absolute_time_t501 + (*TZ300 * 3600)) % 31536000) % 86400) / 3600)) ;
  return h; // Days this year using seconds per day remainder seconds today.
}
*/

// tasks are absolute, adjusted UNIX tme.


long timeJuliet()   // 24 hhmmss local time adjusted for time zone and daylight savings adjust
{
  return ( ( long( hourIs() ) * 10000 ) + ( long( minuteIs() ) * 100 ) + secondIs() );
}

int secondIs()  // Returns the current
{
  return *absolute_time_t501 % SECONDS_PER_MINUTE;
}



int minuteIs()  // Returns the current
{
  return (*absolute_time_t501 % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
}



//B86
int hourIs()  // Returns the current hour
{
   int _hour = ((*absolute_time_t501 % SECONDS_PER_DAY) / SECONDS_PER_HOUR);  // Days and seconds remainder
   
   if(_hour + *TZ300 <= 0)  // midnight and negative
   {
      _hour = _hour + 24 +  *TZ300;
   }
   else
   {
     _hour = _hour + *TZ300;
   }
   if(_hour == 24)
   {
     if((minuteIs() != 0) || (secondIs() != 0)) // Midnight is 24:00:00
     {
       _hour = 0;
     }
   }
  return  _hour;
}

void printTime()
{
  Serial << "UTC ";      // Universal Time Clock
     if( hourIs() <10)
     {
       Serial << "0";
     }
   Serial  << hourIs() << ":" ;
   
     if( minuteIs() <10)
     {
       Serial << "0";
     }
  Serial << minuteIs()  << ":" ;
 
     if( secondIs() <10)
     {
       Serial << "0";
     }
   Serial << secondIs() << "J" << endl;  // Juliet is local time zone
}


void tickTock()
{

  *absolute_time_tULmS600 = (*absolute_time_tUL601 *1000) + (millis() % 1000); // UL absolute seconds to the mS (one thousandeth)
  cS_ticks = (millis() / 100); // will roll itself in 490 days
   if (cS_ticks < PriorcS_ticks) // rolled over
  { 
    PriorcS_ticks = 0;     
  }
  else
  {
    if (cS_ticks > PriorcS_ticks)
    {
      task10xS();
      PriorcS_ticks = cS_ticks;
      
    }
  }
  
  if (((millis() % 1000) > 0) && ((millis() % 1000) < 500) && secondBoundary)
  {
    secondBoundary = false;   // Increment only once. 
    *time_t500 +=1;           // Since startup, no drift adjust
    *absolute_time_t501 +=1;  // Absolute, drift adjusted. For time stamps.
    *absolute_time_tUL601 +=1; 

    // When the absoulte time is adjusted (every day) the task may be repeated if the relative clock is 
    // drifting ahead of absolute time.
    // Adjust *absolute_time_t501 UNIX Time, only in taskEveryDay() or it may be skipped or repeated.
    // On the Controller, the absolute time will be the same at relative time until a time synce is requested from the Bridge.
    // Manually set *absolute_time_t501 by authenticating as bridge user 1, then RX0vgP2,501,1223728457#
    // 1223728457 represents the current UNIX time.

    taskEverySecond();

   if(!(*absolute_time_t501 % SECONDS_PER_MINUTE ))
    {
      taskEveryMinute();

      if(!(*absolute_time_t501 % SECONDS_PER_HOUR ))
      {
        taskEveryHour();

        if(!((*absolute_time_t501 + (*TZ300 * SECONDS_PER_HOUR)) % SECONDS_PER_DAY ))
        {
          taskEveryDay();

          if(!((*absolute_time_t501 + (*TZ300 * SECONDS_PER_HOUR)) % SECONDS_PER_MONTH ))
          {
            taskEveryMonth();
          }

        }
      }
    }

  }
  else  // tickTock()
  {
    if(((millis() % 1000) < 999) && ((millis() % 1000) > 500) && !secondBoundary)
    {
      secondBoundary = true;
    }


  }
  /*
   Serial.print(" QS:");  // Debug
   Serial.print(cS_ticks);  // Debug
   
   Serial.print(" Mills:");  // Debug
   Serial.print(millis());  // Debug
   
   Serial.print(" Sec:");  // Debug
   Serial.print(*time_t500);  // Debug
   
   Serial.print(" Abs:");  // Debug
   Serial.println(*absolute_time_t501);  // Debug
   */


}  // tickTock()



#ifdef FETCH_UNIX_TIME
//////////////////////// Read UNIX time on remote device //////////////////////

//B83
 void sendUNIXtimeQuery(char _tag, int _field)
{
    sendMachinePacketTo(_tag, UNIXTIME_ON_DEV, 'R', 'G', '1', _field, 0, 0, '0');  // 'A' is tracked and reply recorded
     
}


void requestUNIX_TZandDate()
   {   
    
        
       #ifdef DEBUG_UNIX
          Serial << "requestUNIX_TZandDate" << endl;
        #endif
        
         waitUpToMsForReply(100);     
   
           sendUNIXtimeQuery('A',300);    // Get TZ
           waitUpToMsForReply(100);  
           
           sendUNIXtimeQuery('A',301);    // Get week day
           waitUpToMsForReply(100); 
           
           sendUNIXtimeQuery('A',302);    // Get Date  
   }


void readUNIXtime()
  { 
       #ifdef DEBUG_UNIX
            Serial << "Requested UNIX time from:" << UNIXTIME_ON_DEV << endl;
           #endif

   sendUNIXtimeQuery('A',501);     // Request UNIX time
  }
  
  
#endif
