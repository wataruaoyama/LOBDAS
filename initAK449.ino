void initAK449() {
  int regData;
  int b;
  
  cpld.hwConfig = i2cRead(CPLD_ADR, 0x00);
  cpld.deviceConfig0 = i2cRead(CPLD_ADR, 0x01);
  cpld.deviceConfig1 = i2cRead(CPLD_ADR, 0x02);
  
  mono = cpld.deviceConfig0 & 0xC0;
  deviceName = cpld.hwConfig & 0x07;
  DIF = cpld.deviceConfig0 & 0x38;

  // Audio Data Interface modes
  // Default is 32bit I2S intercace
  if (DIF == 0x38) {      // DIF=0x0E
    ak449Chip0.Ctrl1 = 0x8F; 
    ak449Chip1.Ctrl1 = 0x8F;    
  }
  else if (DIF == 0x30) { // DIF=0x0C  
    ak449Chip0.Ctrl1 = 0x8D; 
    ak449Chip1.Ctrl1 = 0x8D;   
  }
  else if (DIF == 0x28) { // DIF=0x0A
    ak449Chip0.Ctrl1 = 0x8B; 
    ak449Chip1.Ctrl1 = 0x88;   
  }
  else if (DIF == 0x20) {  // DIF=0x08
    ak449Chip0.Ctrl1 = 0x89; 
    ak449Chip1.Ctrl1 = 0x89;    
  }
  else if (DIF == 0x18) { // DIF= 0x06
    ak449Chip0.Ctrl1 = 0x87; 
    ak449Chip1.Ctrl1 = 0x87;    
  }
  else if (DIF == 0x10) { // DIF=0x04
    ak449Chip0.Ctrl1 = 0x85; 
    ak449Chip1.Ctrl1 = 0x85;    
  }
  else if (DIF == 0x08) { // DIF=0x02
    ak449Chip0.Ctrl1 = 0x83; 
    ak449Chip1.Ctrl1 = 0x83;    
  }
  else if (DIF == 0x00) { // DIF=0x00
    ak449Chip0.Ctrl1 = 0x81; 
    ak449Chip1.Ctrl1 = 0x81;    
  }

  // De-emphasis Filter Control
  // Defult is De-emphases off
  DEM = cpld.deviceConfig1 & 0x10;
  if (DEM == 0x10) {  // De-emphasis off
    ak449Chip0.Ctrl2 = 0x22; 
    ak449Chip0.Ctrl6 = 0x01;
    ak449Chip1.Ctrl2 = 0x22;
    ak449Chip1.Ctrl6 = 0x01;      
  }
  else {  // De-emphasis on at 44.1kHz
    ak449Chip0.Ctrl2 = 0x20; 
    ak449Chip0.Ctrl6 = 0x00;
    ak449Chip1.Ctrl2 = 0x20;
    ak449Chip1.Ctrl6 = 0x00;
  }

  // Mono/Stereo mode select at each device
  if ( deviceName == 0x07 ) {     // if device is AK4499
    if ( mono == 0xC0 ) {         //  if stereo mode
      ak449Chip0.Ctrl3 = 0x08;  
      ak449Chip0.Ctrl4 = 0x08;
      ak449Chip0.Ctrl7 = 0x20;
    }
    else if ( mono == 0x80 ) {  //    else mono mode
      ak449Chip0.Ctrl3 = 0x08;
      ak449Chip0.Ctrl4 = 0x30;
      ak449Chip0.Ctrl7 = 0x20;
      ak449Chip1.Ctrl3 = 0x0A;
      ak449Chip1.Ctrl4 = 0xC8;
      ak449Chip1.Ctrl7 = 0x20;
    }
  }
  else {  // if device is AK4490/95/95S/93
    if ( mono == 0xC0 ) { // stereo mode
      ak449Chip0.Ctrl3 = 0x00;
      ak449Chip0.Ctrl4 = 0x00;
    }
    else if ( mono == 0x80 ) {  // mono mode
      ak449Chip0.Ctrl3 = 0x08;
      ak449Chip0.Ctrl4 = 0x80;
      ak449Chip1.Ctrl3 = 0x0A;
      ak449Chip1.Ctrl4 = 0x80;     
    }
  }

  DSDF = cpld.deviceConfig1 & 0x01;
  // DSDF: Internal DSD filter setting
  if (DSDF == 0x01) {  // Internal DSD Filter Cut OFF Freq. is Low
    bitWrite(ak449Chip0.Dsd2, 1, 0);
    bitWrite(ak449Chip1.Dsd2, 1, 0);
  }
  else {  // DSDF bit is 1 ( INternal DSD Filter Cut Off Freq. is Hihg
    bitWrite(ak449Chip0.Dsd2, 1, 1);
    bitWrite(ak449Chip1.Dsd2, 1, 1);
  }

  // DSD1,2 register
  bitWrite(ak449Chip0.Dsd1, 7, 1);    // DDM
  bitWrite(ak449Chip1.Dsd1, 7, 1);

  //DDMT
  bitWrite(ak449Chip0.Dsd1, 2, 0);
  bitWrite(ak449Chip1.Dsd1, 2, 0);

  // DSD Data streem select
  // DSDSEL[1]
  b = bitRead(cpld.deviceConfig1, 3);
  bitWrite(ak449Chip0.Dsd2, 0, b);
  bitWrite(ak449Chip1.Dsd2, 0, b);
  // DSDSEL[0]
  b = bitRead(cpld.deviceConfig1, 2);
  bitWrite(ak449Chip0.Dsd1, 0, b);
  bitWrite(ak449Chip1.Dsd1, 0, b);
  
  // DSDD: DSD playback path control
  DSDD = bitRead(cpld.deviceConfig1, 1);
  bitWrite(ak449Chip0.Dsd1, 1, DSDD);
  bitWrite(ak449Chip1.Dsd1, 1, DSDD);

  // Gain Control
  // GC[1]
  GC1 = bitRead(cpld.deviceConfig0, 1);
  bitWrite(ak449Chip0.Ctrl5, 2, GC1);
  bitWrite(ak449Chip1.Ctrl5, 2, GC1);
  // GC[0]
  GC0 = bitRead(cpld.deviceConfig0, 0);
  bitWrite(ak449Chip0.Ctrl5, 1, GC0);
  bitWrite(ak449Chip1.Ctrl5, 1, GC0);

  // SYNCE
  bitWrite(ak449Chip0.Ctrl5, 0, 1);
  bitWrite(ak449Chip1.Ctrl5, 0, 1);
 
  // DSDPATH: DSD data input pin select
  b = bitRead(cpld.deviceConfig0, 2);
  bitWrite(ak449Chip0.Dsd2, 2, b);
  bitWrite(ak449Chip1.Dsd2, 2, b);

  // PW: DAC1,DAC2 power control. 
  bitWrite(ak449Chip0.Ctrl6, 3, 1);
  bitWrite(ak449Chip0.Ctrl6, 2, 1);
  bitWrite(ak449Chip1.Ctrl6, 3, 1);
  bitWrite(ak449Chip1.Ctrl6, 2, 1);
  
  // ADPE
  ak449Chip0.Ctrl8 = 0xE0;
  ak449Chip1.Ctrl8 = 0xE0;

  // Volume
  
}
