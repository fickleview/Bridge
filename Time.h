// Built on 2.1.204 
#ifndef TIME_H
#define TIME_H

extern   void tickTock();

extern   byte hourNow(long);
extern   byte minuteNow(long);

extern   long midnightUnixTimeWas();
extern   long timeJuliet();
extern   int secondIs();

extern   int minuteIs();
extern   int hourIs();
extern   void printTime();

extern   void pingEveryMSeconds(long, long);
extern   void waitUpToMsForReply(int);
extern   void requestUNIX_TZandDate();

extern   void readUNIXtime();


#endif // TIME_H
