// Last modified 
// Should not be modified

// &&&&&&&&&&&&&&&&&  Voltage on Pin  &&&&&&&&&&&&&&&&&&&&&&&&&



// Battery is connected via two 5.1v Zener diodes, and therefore 0 = 10.2V or 102
// In practice 9.6 volts = 97

// We have 10 bits (1024) to reprsent 5v range of A/D = 1024/5 = 204.8 0r 205 bits per volt

// Returns 10x the voltage on pin +- the offset.

#ifdef VOLTS
int getVoltsOnPin(byte pin, int offset)
{
  int volts; 
  volts = analogRead(pin) * 10;  // 00 to 50 = 0 to 5.0 volts
  volts = ( volts / 205 ) + offset; // Add/Subtract the volts offset from zero.

  return (volts);

}
#endif // VOLTS
