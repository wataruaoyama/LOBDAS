void inputSelection() {
  bool state = digitalRead(inputSwitch);

  if ( inswState == HIGH && state == LOW) {
    count++;
    if ( count == 1 ) {
      bitWrite(ak449Chip0.Ctrl2, 0, 1);
      bitWrite(ak449Chip1.Ctrl2, 0, 1); 
      i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute ON
      if ( mono == 0x80 ) {
        i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);  // Soft mute ON
      }      
      i2cWrite(CPLD_ADR, 0x00, 0x00);     // Change Input to USB
      bitWrite(ak449Chip0.Ctrl2, 0, 0);
      bitWrite(ak449Chip1.Ctrl2, 0, 0);       
      i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute OFF
      if ( mono == 0x80 ) {
        i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);  // Soft mute OFF
      }
      Serial.println("USB INPUT Selected");
    } else if (count == 2) {
      bitWrite(ak449Chip0.Ctrl2, 0, 1); 
      bitWrite(ak449Chip1.Ctrl2, 0, 1); 
      i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute ON
      if ( mono == 0x80 ) {
        i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);  // Soft mute ON
      }
      i2cWrite(CPLD_ADR, 0x00, 0x08);      // Change Inut to RJ45
      bitWrite(ak449Chip0.Ctrl2, 0, 0); 
      bitWrite(ak449Chip1.Ctrl2, 0, 0); 
      i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute OFF
      if ( mono == 0x80 ) {
        i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);  // Soft mute OFF
      }
      Serial.println("RJ45 INPUT Seleted");
    } else if (count == 3) {
      bitWrite(ak449Chip0.Ctrl2, 0, 1); 
      bitWrite(ak449Chip1.Ctrl2, 0, 1); 
      i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute ON
      if ( mono == 0x80 ) {
        i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);  // Soft mute ON
      }
      i2cWrite(CPLD_ADR, 0x00, 0x10);      // Change Inut to RJ45
      bitWrite(ak449Chip0.Ctrl2, 0, 0); 
      bitWrite(ak449Chip1.Ctrl2, 0, 0); 
      i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute OFF
      if ( mono == 0x80 ) {
        i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);  // Soft mute OFF
      }
      count = 0;
      Serial.println("XH INPUT Selected");
    }
  }
  inswState = state;
}
