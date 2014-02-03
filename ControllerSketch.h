// Your controller header here
// Built on 2.1.206

#ifndef  CONTROLLER_SKETCH_H
#define  CONTROLLER_SKETCH_H



// Place your function prototypes, defines and variables that will be accessed by others here


#define ALERT_CONTROLLER 'X'

#define CHAR_FIELD_SIZE   2
#define BYTE_FIELD_SIZE   3
#define INT_FIELD_SIZE   11
#define UINT_FIELD_SIZE   0
#define LONG_FIELD_SIZE   4
#define U_LONG_FIELD_SIZE 2
#define DOUBLE_FIELD_SIZE 0


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
