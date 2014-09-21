// Last modified 
// Your controller header here

#ifndef  CONTROLLER_SKETCH_H
#define  CONTROLLER_SKETCH_H



// Place your function prototypes, defines and variables that will be accessed by others here


#define ALERT_CONTROLLER 'X'


const int CHAR_FIELD_SIZE =   2;
const int  BYTE_FIELD_SIZE =  3;
const int  INT_FIELD_SIZE =  11;
const int  UINT_FIELD_SIZE =  0;
const int  LONG_FIELD_SIZE =  4;
const int  U_LONG_FIELD_SIZE= 2;
const int  DOUBLE_FIELD_SIZE= 0;

const int  DS18B20_PIN= 3;

// Prototypes


extern void userSetup();
extern void userCommand(char);

extern void userCommand(char);
extern void userHandleNotification();
extern void task10xS();
extern void taskEverySecond();
extern void taskEveryMinute();

extern void taskEveryHour();
extern void taskEveryDay();
extern void taskEveryMonth();

extern void handleTrackingTag();

#endif //CONTROLLER_SKETCH_H
