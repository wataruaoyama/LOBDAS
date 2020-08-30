/* */
 void changeFilter() {
  bool state = digitalRead(filterSwitch);

  if ( DSDON == 0x01 ) {
    oled.print(filterBlank);
  } else if ( buttonState == HIGH && state == LOW) {
    cnt++;
    if ( cnt == 1 ) {
      bitWrite(ak449Chip0.Ctrl3, 0, 0);
      bitWrite(ak449Chip0.Ctrl2, 5, 0);
      bitWrite(ak449Chip0.Ctrl4, 0, 0);
      if ( mono == 0x80) {
        bitWrite(ak449Chip1.Ctrl3, 0, 0);
        bitWrite(ak449Chip1.Ctrl2, 5, 0);
        bitWrite(ak449Chip1.Ctrl4, 0, 0);        
      }
    } else if (cnt == 2) {
      bitWrite(ak449Chip0.Ctrl3, 0, 1);
      if ( mono == 0x80) {
        bitWrite(ak449Chip1.Ctrl3, 0, 1);       
      }
    } else if ( cnt == 3 ) {
      bitWrite(ak449Chip0.Ctrl3, 0, 0);
      bitWrite(ak449Chip0.Ctrl2, 5, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl3, 0, 0);
        bitWrite(ak449Chip1.Ctrl2, 5, 1);        
      }
    } else if ( cnt == 4 ) {
      bitWrite(ak449Chip0.Ctrl3, 0, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl3, 0, 1);        
      }
    } else if ( cnt == 5 ) {
      bitWrite(ak449Chip0.Ctrl3, 0, 0);
      bitWrite(ak449Chip0.Ctrl2, 5, 0);
      bitWrite(ak449Chip0.Ctrl4, 0, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl3, 0, 0);
        bitWrite(ak449Chip1.Ctrl2, 5, 0);
        bitWrite(ak449Chip1.Ctrl4, 0, 1);        
      }
    } else if ( cnt == 6 ) {
      bitWrite(ak449Chip0.Ctrl2, 5, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl2, 5, 1);
      }
      cnt = 0;
    }
  }
  buttonState = state;
  i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);
  i2cWrite(AK449_Chip0, 0x02, ak449Chip0.Ctrl3);
  i2cWrite(AK449_Chip0, 0x05, ak449Chip0.Ctrl4);
  if ( mono == 0x80 ) {
    i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);
    i2cWrite(AK449_Chip1, 0x02, ak449Chip1.Ctrl3);
    i2cWrite(AK449_Chip1, 0x05, ak449Chip1.Ctrl4);    
  }
}
