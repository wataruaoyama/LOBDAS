void messageOut() {
  cpld.sampleRate = i2cRead(CPLD_ADR, 0x03);
  DSDON = cpld.sampleRate & 0x01;
  FS = cpld.sampleRate & 0x3C;
  DSD64 = cpld.sampleRate & 0x42;

  displayPlayMode();
  displayATT(volumeCounter);
  displayFs();
  displayDigitalFilter();
  displayInputInterface();
}

void displayPlayMode() {
  
  if ( DSDON == 0x01) {
    oled.setCursor(0, 0);
    oled.print("DSD");
  } else if ( DSDON == 0x00 ){
    oled.setCursor(0, 0);
    oled.print("PCM");
  }
}

void displayATT(int vin) {
  int level;
  int decimals;
  
  decimals = vin & 0x01;
  level = vin>>1;
  level -= 127;
  level = abs(level);

  oled.setCursor(13, 0);

  if ( (0 <= level) && (level <10)) {
    int onesPlace = level % 10;
    oled.print("  ");
    oled.print(onesPlace);
    if ( decimals == 1) {
      oled.print(".0dB");
    }
    else {
      oled.print(".5dB");
    }
  }
  else if ( (9 < level) && (level <100) ) {
    int tensPlace = level % 100;
    oled.print(" ");
    oled.print(tensPlace);
    if ( decimals == 1) {
      oled.print(".0dB");
    }
    else {
      oled.print(".5dB");
    }
  }
  else if ( level > 99 ) {
    oled.print(level);
    if ( decimals == 1) {
      oled.print(".0dB");
    }
    else {
      oled.print(".5dB");
    }
  }
}

void displayFs() {
  oled.setCursor(3, 0);

  if ( DSDON == 0x00 ) {
    if ( FS == 0x00 ) {
      oled.print(freq32);
    } else if ( FS == 0x04 ) {
      oled.print(freq44);
    } else if ( FS == 0x08 ) {
      oled.print(freq48);
    } else if ( FS == 0x0C ) {
      oled.print(freq88 );
    } else if ( FS == 0x10 ) {
      oled.print(freq96);
    } else if ( FS == 0x14 ) {
      oled.print(freq176);
    } else if ( FS == 0x18 ) {
      oled.print(freq192);
    } else if ( FS == 0x1C ) {
      oled.print(freq352);
    } else if ( FS == 0x20 ) {
      oled.print(freq384);
    }
  } else if (DSDON == 0x01 ) {
    if ( DSD64 == 0x00 ) {
      oled.print(freqDsd64);
    } else if ( DSD64 == 0x02 ) {
      oled.print(freqDsd128);
    } else if ( DSD64 == 0x40 ) {
      oled.print(freqDsd256);
    } else if ( DSD64 == 0x42 ) {
      oled.print(freqDsd512);
    }
  }
}

void displayDigitalFilter() {
  oled.setCursor(0, 1);

  if ( DSDON == 0x01 ) {
    oled.print(filterBlank);
  } else if ( DSDON == 0x00 ) {
    if ( cnt == 1 ) {
      oled.print(Sharp);      
    } else if (cnt == 2) {
      oled.print(Slow);      
    } else if ( cnt == 3 ) {
      oled.print(ShortDelaySharp);     
    } else if ( cnt == 4 ) {
      oled.print(ShortDelaySlow);
    } else if ( cnt == 5 ) {
      oled.print(SuperSlow);
    } else if ( cnt == 6 || cnt == 0) {
      oled.print(LowDispersion);
    }
  }
}

void displayInputInterface() {
  oled.setCursor(17, 1);
  
  if ( count == 1 ) {       // USB
    oled.print("[U]");
  }
  else if ( count == 2 ) {  // RJ45
    oled.print("[R]");
  }
  else if ( count == 3 ) {  // XH Connector
    oled.print("[X]");
  }
  else {
    oled.print("[X]");
  }
}
