// 2014-11-05


void handleDisplay()  // MRMP packet type 'D' received or local see documentation
{
 
 #ifdef LCD_DISPLAY
 lcd.setCursor(parm2,parm3);
 
 //parm1 Field width
 //parm2 Column
 //parm3 Row
 //parm4 Value
 //
 //
 
  if(commandParameters==4)
             {
              if(parm1) // Field width to pad with zeros
              {
               
             
               for( int _fld = parm1; _fld > 0; _fld--)
               {
                lcd.print("0");
               }
            
            int numDigitsCursor = parm2 + parm1; //
            
            if(parm4 < 10)
             {
              numDigitsCursor -= 1;
             }
            else if(parm4 < 100)
             {
              numDigitsCursor -= 2;
             }
             else if (parm4 < 1000)
                  {
                   numDigitsCursor -= 3;
                  }
                  else if (parm4 < 10000)
                  {
                   numDigitsCursor -= 4;
                  }
              
            
               lcd.setCursor(numDigitsCursor,parm3); // for leading zeros then print the value parm4
              }
              lcd.print(parm4);     
            }
            else  // 5 parameters
            {
             *(parameterPointerArray[5]+parm1)=0; // Null terminate string to avoid printing the '#'
              lcd.print(parameterPointerArray[5]);
            } 
  
  #endif LCD_DISPLAY
}

 #ifdef LCD_DISPLAY
void printIntx10ToDec(int _x10)  // Singned interger x10
{
 if(_x10 < 100)
 {
  lcd.print(" ");
 }

 if(_x10 >= 0)
 {
  lcd.print(" ");
 }
 
 if(_x10/10 == 0 && _x10 < 0)
 {
  lcd.print("-");
 }
 
 lcd.print(_x10/10);
 lcd.print(".");
 lcd.print(abs( _x10 % 10));
}


  #endif LCD_DISPLAY
/*
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
*/

