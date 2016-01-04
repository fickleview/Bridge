// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol

extern void printLCDtime(int,int);
extern void printTempDec(int);

extern void handleDisplay();  // MRMP packet type 'D' received
extern void displayTempOnAt(char, int, int, int);

#ifdef LCD_DISPLAY

// IIC / I2C 2004 204 20 x 4 Character LCD Display Module Yellow Green  Banggood SKU: SKU107152
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Type 1  Set the LCD I2C address

// See SetupAndLoop for setup
#endif

/*
GND - GND
VCC - 5V
SDA - ANALOG Pin 4
SCL - ANALOG pin 5
On most Arduino boards, SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5. On the Arduino Mega, SDA is digital pin 20 and SCL is 21.
NOTE: The Blue Potentiometer (Photo) adjusts Contrast. If you don't see any characters, adjust it. Start clockwise and back down to where the characters are bright and the background does not have boxes behind the characters.
*/
