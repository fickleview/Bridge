// Your controller header here
// Built on 2.1.206

#ifndef  CONTROLLER_SKETCH_H
#define  CONTROLLER_SKETCH_H

 
// Place your function prototypes, defines and variables that will be accessed by others here

#define ALERT_CONTROLLER 'X'

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
