void initDisplay() {
  
  oled.begin(20, 2);
  oled.clear();
//  delay(1000);
  oled.setCursor(0,0);
  
  if ( deviceName == 0x07 ) {
    oled.print(ak4499);
  }
  else if ( deviceName == 0x06 ) {
    oled.print(ak4493);
  }
  else if ( deviceName == 0x05 ) {
    oled.print(ak4495);
  }
  else if ( deviceName == 0x04 ) {
    oled.print(ak4490);
  }
  else {
   oled.print(others);
  }
  
  oled.setCursor(6,0);
  if ( mono == 0xC0 ) {
    oled.print(" Stereo");
  }
  else if ( mono == 0x80 ) {
    oled.print(" Mono");
  }
  oled.setCursor(0,1);
  oled.print("by LINUXCOM");
  delay(3000);  
  oled.clear();
  
  oled.setCursor(0,0);
  oled.print("DIF:");
  oled.setCursor(4,0);
  if ( DIF == 0x38 ) {
    oled.print(audioIF7);
  }
  else if ( DIF == 0x30 ) {
    oled.print(audioIF6);
  }
  else if ( DIF == 0x28 ) {
    oled.print(audioIF5);
  }
  else if ( DIF == 0x20 ) {
    oled.print(audioIF4);
  }
  else if ( DIF == 0x18 ) {
    oled.print(audioIF3);
  }
  else if ( DIF == 0x10 ) {
    oled.print(audioIF2);
  }
  else if ( DIF == 0x08 ) {
    oled.print(audioIF1);
  }
  else if ( DIF == 0x00 ) {
    oled.print(audioIF0);
  }

  oled.setCursor(0,1);
  oled.print("DSDD:");
  oled.setCursor(5,1);
  if ( DSDD == HIGH ) {
    oled.print(dsdBypass);
  }
  else {
    oled.print(dsdNormal);
  }
  delay(3000);
  oled.clear();

  oled.setCursor(0,0);
  if ( deviceName == 0x07 ) {
    oled.print("COL:");
    oled.setCursor(4,0);
    if (( GC1 == HIGH ) && ( GC0 == HIGH )) {
      oled.print(currentOut3);
    }
    else if (( GC1 == HIGH ) && ( GC0 == LOW )) {
      oled.print(currentOut2);
    } 
    else if (( GC1 == LOW ) && ( GC0 == HIGH )) {
      oled.print(currentOut1);
    }
    else if (( GC1 == LOW ) && ( GC0 == LOW )) {
      oled.print(currentOut0);
    }
  }
  else {
    oled.print("VOL:");
    oled.setCursor(4,0);
    if (( GC1 == HIGH ) && ( GC0 == HIGH )) {
      oled.print(outputLevel3);
    }
    else if (( GC1 == HIGH ) && ( GC0 == LOW )) {
      oled.print(outputLevel2);
    } 
    else if (( GC1 == LOW ) && ( GC0 == HIGH )) {
      oled.print(outputLevel1);
    }
    else if (( GC1 == LOW ) && ( GC0 == LOW )) {
      oled.print(outputLevel0);
    }
  }
  oled.setCursor(0,1);
  oled.print("DEM:");
  oled.setCursor(4,1);
  if ( DEM == 0x10 ) {
    oled.print(demOff);
  }
  else {
    oled.print(demOn);
  }
  oled.setCursor(8,1);
  oled.print("DSDF:");
  if ( DSDF == 0x01 ) {
    oled.print("Low");
  } else {
    oled.print("High");
  }
  delay(3000);
  oled.clear();
}
