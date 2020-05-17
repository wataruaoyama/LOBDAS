void initDisplay() {
  
  oled.begin(20, 2);
  oled.clear();
  delay(1000);
  oled.setCursor(2,0);
  
  if ( deviceName == 0x07 ) {
    oled.print("AKM AK4499 SYSTEM");
  }
  else if ( deviceName == 0x06 ) {
    oled.print("AKM AK4493 SYSEM");
  }
  else if ( deviceName == 0x05 ) {
    oled.print("AKM AK4495S SYSTEM");
  }
  else if ( deviceName == 0x04 ) {
    oled.print("AKM AK4490 SYSTEM");
  }
  else {
   oled.print("AKM DAC SYSTEM");
  }
  oled.setCursor(3,1);
  oled.print("by LINUXCOM");
  delay(3000);  
  oled.clear();
}
