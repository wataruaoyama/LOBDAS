void volumeControl() {
  boolean Up,Down;
  Up = digitalRead(upSwitch);
  Down = digitalRead(downSwitch);
  
  if ( !Up ) {
    if ( volumeCounter == 255 ){
      volumeCounter = volumeCounter;
    }
    else {
      volumeCounter++;
    }
  }
  else if ( !Down ) {
    if ( volumeCounter == 0 ) {
      volumeCounter = volumeCounter;
    }
    else {
      volumeCounter--;
    }    
  }
  i2cWrite(AK449_Chip0, 0x03, volumeCounter);
  i2cWrite(AK449_Chip0, 0x04, volumeCounter);
  i2cWrite(AK449_Chip0, 0x0C, volumeCounter);
  i2cWrite(AK449_Chip0, 0x0D, volumeCounter);  
  if ( mono == 0x80 ) {
    i2cWrite(AK449_Chip1, 0x03, volumeCounter);
    i2cWrite(AK449_Chip1, 0x04, volumeCounter);
    i2cWrite(AK449_Chip1, 0x0C, volumeCounter);
    i2cWrite(AK449_Chip1, 0x0D, volumeCounter);
  }
}
