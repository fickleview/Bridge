// 2015-09-19


void handleDisplay()  // MRMP packet type 'D' received or local see documentation
{
 
 #ifdef LCD_DISPLAY
 lcd.setCursor(parm2,parm3);
 
  if(commandParameters==4)
            {
              lcd.print(parm4);     
            }
            else  // 5 parameters
            {
             *(parameterPointerArray[5]+parm1)=0; // Null terminate string to avoid printing the '#'
              lcd.print(parameterPointerArray[5]);
            } 
 #endif LCD_DISPLAY   
 
}



void printIntx10ToDec(int _x10)
{
 if(_x10 < 100)
 {
  lcd.print(" ");
 }

 if(_x10 >= 0)
 {
  lcd.print(" ");
 }
 
 lcd.print(_x10/10);
 lcd.print(".");
 lcd.print( _x10 % 10);
}


void printLCDtime(int col, int line)
{
 uint8_t _hr  = hourIs();
 uint8_t _min = minuteIs();
 
  lcd.setCursor(col,line); //Start at character
  if (_hr <10)
  {
  lcd.print(0);
  }
  lcd.print(_hr);
  
  lcd.print(":");
  
  if (_min <10)
  {
  lcd.print(0);
  }
  lcd.print(_min);
}
