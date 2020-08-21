void blynkDisplayOut() {
  cpld.sampleRate = i2cRead(CPLD_ADR, 0x03);
  DSDON = cpld.sampleRate & 0x01;
  FS = cpld.sampleRate & 0x3C;
  DSD64 = cpld.sampleRate & 0x42;

  if (blynkModeButton == 0) {
    blynkDisplayPlayMode();
    blynkDisplayFs();
    blynkDisplayDigitalFilter();
  } else {
    blynkDisplayATT(volumeCounter);
    blynkDisplayInputInterface();
  }
}

void blynkDisplayPlayMode() {
  
  if ( DSDON == 0x01) {
    lcd.print(0, 0, "DSD");
  } else if ( DSDON == 0x00 ){
    lcd.print(0, 0, "PCM");
  }
}

void blynkDisplayATT(int vin) {
  int level;
  int decimals;
  
  decimals = vin & 0x01;
  level = vin>>1;
  level -= 127;
  level = abs(level);

  lcd.print(0, 0, "ATT:");
  lcd.print(13, 0, "  ");

  if ( (0 <= level) && (level <10)) {
    int onesPlace = level % 10;
    lcd.print(4, 0, "  ");
    lcd.print(6, 0, onesPlace);
    if ( decimals == 1) {
        lcd.print(7, 0, ".0[dB]");
    }
    else {
        lcd.print(7, 0, ".5[dB]");
    }
  }
  else if ( (9 < level) && (level <100) ) {
    int tensPlace = level % 100;
    lcd.print(4, 0, " ");
    lcd.print(5, 0, tensPlace);
    if ( decimals == 1) {
      lcd.print(7, 0, ".0[dB]");
    }
    else {
      lcd.print(7, 0, ".5[dB]");
    }
  }
  else if ( level > 99 ) {
    lcd.print(4, 0, level);
    if ( decimals == 1) {
      lcd.print(7, 0, ".0[dB]");
    }
    else {
      lcd.print(7, 0, ".5[dB]");
    }
  }
}

void blynkDisplayFs() {
  if ( DSDON == 0x00 ) {
    if ( FS == 0x00 ) {
      lcd.print(3, 0, freq32);
      lcd.print(12, 0, "    ");
    } else if ( FS == 0x04 ) {
      lcd.print(3, 0, freq44);
      lcd.print(12, 0, "    ");
    } else if ( FS == 0x08 ) {
      lcd.print(3, 0, freq48);
      lcd.print(12, 0, "    ");
    } else if ( FS == 0x0C ) {
      lcd.print(3, 0, freq88);
      lcd.print(12, 0, "    ");
    } else if ( FS == 0x10 ) {
      lcd.print(3, 0, freq96);
      lcd.print(12, 0, "    ");
    } else if ( FS == 0x14 ) {
      lcd.print(3, 0, freq176);
      lcd.print(12, 0, "    ");
    } else if ( FS == 0x18 ) {
      lcd.print(3, 0, freq192);
      lcd.print(12, 0, "    ");
    } else if ( FS == 0x1C ) {
      lcd.print(3, 0, freq352);
      lcd.print(12, 0, "    ");
    } else if ( FS == 0x20 ) {
      lcd.print(3, 0, freq384);
      lcd.print(12, 0, "    ");
    }
  } else if (DSDON == 0x01 ) {
    if ( DSD64 == 0x00 ) {
      lcd.print(3, 0, freqDsd64);
      lcd.print(12, 0, "    ");
    } else if ( DSD64 == 0x02 ) {
      lcd.print(3, 0, freqDsd128);
      lcd.print(12, 0, "    ");
    } else if ( DSD64 == 0x40 ) {
      lcd.print(3, 0, freqDsd256);
      lcd.print(12, 0, "    ");
    } else if ( DSD64 == 0x42 ) {
      lcd.print(3, 0, freqDsd512);
      lcd.print(12, 0, "    ");
    }
  }
}

void blynkDisplayDigitalFilter() {
  if ( DSDON == 0x01 ) {
    lcd.print(0, 1, filterBlank);
  } else if ( DSDON == 0x00 ) {
    if ( cnt == 1 ) {
      lcd.print(0, 1, Sharp);      
    } else if (cnt == 2) {
      lcd.print(0, 1, Slow);      
    } else if ( cnt == 3 ) {
      lcd.print(0, 1, ShortDelayShrp);     
    } else if ( cnt == 4 ) {
      lcd.print(0,1, ShortDelaySlow);
    } else if ( cnt == 5 ) {
      lcd.print(0, 1, SuperSlow);
    } else if ( cnt == 6 || cnt == 0) {
      lcd.print(0, 1, LowDispersion);
    }
  }
}

void blynkDisplayInputInterface() {
  lcd.print(0, 1, "INPUT:");  
  if ( count == 1 ) {       // USB
    lcd.print(6, 1, "USB       ");
  }
  else if ( count == 2 ) {  // RJ45
    lcd.print(6, 1, "RJ45      ");
  }
  else if ( count == 3 ) {  // XH Connector
    lcd.print(6, 1, "XH        ");
  }
  else {
    lcd.print(6, 1, "XH        ");
  }
}
