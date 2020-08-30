void changePlayMode(){
  int DSD = digitalRead(DP);

  if (blynkMuteButton == 1) {
    bitWrite(ak449Chip0.Ctrl2, 0, 1); 
    i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute ON
    digitalWrite(pwLED,LOW);
  } else {
    bitWrite(ak449Chip0.Ctrl2, 0, 0); 
    i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute OFF   
   digitalWrite(pwLED,HIGH);
  }
  
//  bitWrite(ak449Chip0.Dsd1, 0, 0);
//  bitWrite(ak449Chip0.Dsd2, 0, 1);
//  bitWrite(ak449Chip1.Dsd1, 0, 0);
//  bitWrite(ak449Chip1.Dsd2, 0, 1);
//  i2cWrite(AK449_Chip0, 0x06, ak449Chip0.Dsd1);
//  i2cWrite(AK449_Chip0, 0x09, ak449Chip0.Dsd2);
      
  if (DSD64 == 0x42) {          // if DSD512
    bitWrite(ak449Chip0.Dsd1, 0, 1);
    bitWrite(ak449Chip0.Dsd2, 0, 1);
    bitWrite(ak449Chip1.Dsd1, 0, 1);
    bitWrite(ak449Chip1.Dsd2, 0, 1);
  } else if (DSD64 == 0x40) {   // if DSD256
    bitWrite(ak449Chip0.Dsd1, 0, 0);
    bitWrite(ak449Chip0.Dsd2, 0, 1);
    bitWrite(ak449Chip1.Dsd1, 0, 0);
    bitWrite(ak449Chip1.Dsd2, 0, 1);
//  } else if (DSD64 == 0x02) {   // if DSD128
  } else if ((DSD64 == 0x02) or (DSD64 == 0x00)) {   // DSD128かDSD64の場合
    bitWrite(ak449Chip0.Dsd1, 0, 1);
    bitWrite(ak449Chip0.Dsd2, 0, 0);
    bitWrite(ak449Chip1.Dsd1, 0, 1);
    bitWrite(ak449Chip1.Dsd2, 0, 0);
//  } else if (DSD64 == 0x00) {   // if DSD64
//    bitWrite(ak449Chip0.Dsd1, 0, 0);
//    bitWrite(ak449Chip0.Dsd2, 0, 0);
//    bitWrite(ak449Chip1.Dsd1, 0, 0);
//    bitWrite(ak449Chip1.Dsd2, 0, 0);
  }
  i2cWrite(AK449_Chip0, 0x06, ak449Chip0.Dsd1);
  i2cWrite(AK449_Chip0, 0x09, ak449Chip0.Dsd2);
  if (mono == 0x80) {
    i2cWrite(AK449_Chip1, 0x06, ak449Chip0.Dsd1);
    i2cWrite(AK449_Chip1, 0x09, ak449Chip0.Dsd2);
  }

  if ( (prevMode == 1) && (DSD == 0)  ) {   // PCM mode
//    bitWrite(ak449Chip0.Ctrl2, 0, 1); 
//    i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute ON
    bitWrite(ak449Chip0.Ctrl3, 7, 0);
    i2cWrite(AK449_Chip0, 0x02, ak449Chip0.Ctrl3);  // Set "0" to DP bit
    delay(0);
    bitWrite(ak449Chip0.Ctrl1, 0, 0);
    i2cWrite(AK449_Chip0, 0x00, ak449Chip0.Ctrl1);  // Set "0" to RSTN bit. Rset
    delay(180);
    bitWrite(ak449Chip0.Ctrl1, 0, 1);
    i2cWrite(AK449_Chip0, 0x00, ak449Chip0.Ctrl1);  // Set "1" to RSTN bit. Normal Operation
//    bitWrite(ak449Chip0.Ctrl2, 0, 0); 
//    i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute OFF
    if (mono == 0x80) {   // Dual mono mode
//      bitWrite(ak449Chip1.Ctrl2, 0, 1); 
//      i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);  // Soft mute ON
      bitWrite(ak449Chip1.Ctrl3, 7, 0);
      i2cWrite(AK449_Chip1, 0x02, ak449Chip1.Ctrl3);  // Set "0" to DP bit
      delay(0);
      bitWrite(ak449Chip1.Ctrl1, 0, 0);
      i2cWrite(AK449_Chip1, 0x00, ak449Chip1.Ctrl1);  // Set "0" to RSTN bit. Rset
      delay(180);
      bitWrite(ak449Chip1.Ctrl1, 0, 1);
      i2cWrite(AK449_Chip1, 0x00, ak449Chip1.Ctrl1);  // Set "1" to RSTN bit. Normal Operation
//      bitWrite(ak449Chip1.Ctrl2, 0, 0); 
//      i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);  // Soft mute OFF
    }
    Serial.println("Internal Timing Reset by PCM");
  } else if ( (prevMode == 0) && (DSD == 1) ) {   // DSD mode
//    bitWrite(ak449Chip0.Ctrl2, 0, 1); 
//    i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute ON
    bitWrite(ak449Chip0.Ctrl3, 7, 1);
    i2cWrite(AK449_Chip0, 0x02, ak449Chip0.Ctrl3);  // Set "1" to DP bit
    delay(0);
    bitWrite(ak449Chip0.Ctrl1, 0, 0);
    i2cWrite(AK449_Chip0, 0x00, ak449Chip0.Ctrl1);  // Set "0" to RSTN bit. Rset
    delay(180);
    bitWrite(ak449Chip0.Ctrl1, 0, 1);
    i2cWrite(AK449_Chip0, 0x00, ak449Chip0.Ctrl1);  // Set "1" to RSTN bit. Normal Operation
//    bitWrite(ak449Chip0.Ctrl2, 0, 0); 
//    i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute OFF
    if (mono == 0x80) {   // Dusl mono mode
//      bitWrite(ak449Chip1.Ctrl2, 0, 1); 
//      i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);  // Soft mute ON
      bitWrite(ak449Chip1.Ctrl3, 7, 1);
      i2cWrite(AK449_Chip1, 0x02, ak449Chip1.Ctrl3);  // Set "1" to DP bit
      delay(0);
      bitWrite(ak449Chip1.Ctrl1, 0, 0);
      i2cWrite(AK449_Chip1, 0x00, ak449Chip1.Ctrl1);  // Set "0" to RSTN bit. Rset
      delay(180);
      bitWrite(ak449Chip1.Ctrl1, 0, 1);
      i2cWrite(AK449_Chip1, 0x00, ak449Chip1.Ctrl1);  // Set "1" to RSTN bit. Normal Operation
//      bitWrite(ak449Chip1.Ctrl2, 0, 0); 
//      i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);  // Soft mute OFF
    }
    Serial.println("Internal Timing Reset by DSD");
  }
  prevMode = DSD;
}
