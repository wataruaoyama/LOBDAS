/*************************************************************
  AK449xの初期化
  *************
  
  レジスタマップをメンバーとする構造体変数の初期化.
  デバイスに設定値を書き込みのはinitRegisterで行う．

  設定用のヘッダーピンの状態を読み、それに応じてレジスタの設定を行う．
  ヘッダーピンの設定状態はCPLDのレジスタ00H-02Hに割り当てられる．

               D7   D6   D5   D4   D3     D2       D1       D0
      +------+----+----+----+----+----+--------+--------+--------+
  00H |HWCONF|OPT1|OPT2| -- | -- | -- |devName2|devName1|devMame0|
      +------+----+----+----+----+----+--------+--------+--------+
      
               　D7    D6    D5   D4   D3   D2     D1   D0
      +-------+-----+-----+----+----+----+-------+----+----+
  01H |DVCONF0|MONO1|MONO0|DIF2|DIF1|DIF0|DSDPATH|GC1 |GC0 |
      +-------+-----+-----+----+----+----+-------+----+----+
      
                D7   D6   D5   D4    D3      D2     D1   D0
      +-------+----+----+----+----+-------+-------+----+----+
  02H |DVCONF1|CHLR| -- | -- |DEMP|DSDSEL1|DSDSEL0|DSDD|DSDF|
      +-------+----+----+----+----+-------+-------+----+----+ 
      
 *************************************************************/
void initAK449() {
  int regData;
  int b;
  
  cpld.hwConfig = i2cRead(CPLD_ADR, 0x00);
  cpld.deviceConfig0 = i2cRead(CPLD_ADR, 0x01);
  cpld.deviceConfig1 = i2cRead(CPLD_ADR, 0x02);
  
  mono = cpld.deviceConfig0 & 0xC0;
  deviceName = cpld.hwConfig & 0x07;
  DIF = cpld.deviceConfig0 & 0x38;

  /* Audio Data Interface modesの設定 */
  /* デフォルトは32bit I2S intercace */
  // DIF[2:0]=(111)
  if (DIF == 0x38) {
    // 32-bit I2S
    ak449Chip0.Ctrl1 = 0x8F; 
    ak449Chip1.Ctrl1 = 0x8F;    
  }
  // DIF[2:0]=(110)
  else if (DIF == 0x30) {
    // 32-bit 左詰め
    ak449Chip0.Ctrl1 = 0x8D; 
    ak449Chip1.Ctrl1 = 0x8D;   
  }
  // DIF[2:0]=(101)
  else if (DIF == 0x28) {
    // 32-bit 右詰め
    ak449Chip0.Ctrl1 = 0x8B; 
    ak449Chip1.Ctrl1 = 0x88;   
  }
  // DIF[2:0]=(100)
  else if (DIF == 0x20) {
    // 24-bit 右詰め
    ak449Chip0.Ctrl1 = 0x89; 
    ak449Chip1.Ctrl1 = 0x89;    
  }
  // DIF[2:0]=(011)
  else if (DIF == 0x18) {
    // 24-bit I2S/16-bit I2S
    ak449Chip0.Ctrl1 = 0x87; 
    ak449Chip1.Ctrl1 = 0x87;    
  }
  // DIF[2:0]=(010)
  else if (DIF == 0x10) {
    // 24-bit 左詰め
    ak449Chip0.Ctrl1 = 0x85; 
    ak449Chip1.Ctrl1 = 0x85;    
  }
  // DIF[2:0]=(001)
  else if (DIF == 0x08) {
    // 20-bit 右詰め
    ak449Chip0.Ctrl1 = 0x83; 
    ak449Chip1.Ctrl1 = 0x83;    
  }
  // DIF[2:0]=(000)
  else if (DIF == 0x00) {
    // 16-bit 右詰め
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

  /* 
   * ステレオ/デュアルモノ時のMONO1/2,SELLR1/2,INVL1/2,INVR1/2ビットの設定
   * 下表はDAC-A4499 + LPFボード,DSD-A4490,PCM-A4495/Sボードでの設定で、
   * 他の構成においては変更する必要がある．
   */
  // Mono/Stereo mode select at each device
  if ( deviceName == 0x07 ) {     // if device is AK4499
    // ------+-----+------+-----+-----+-----+------+-----+-----
    // CHIP0 |MONO1|SELLR1|INVL1|INVR1|MONO2|SELLR2|INVL2|INVR2
    // ------+-----+------+-----+-----+-----+------+-----+-----
    // STEREO|  1  |   0  |  0  |  0  |  1  |  1   |  0  |  0  
    // ------+-----+------+-----+-----+-----+------+-----+-----
    // MONO  |  1  |   0  |  0  |  0  |  1  |  0   |  1  |  1  
    // ------+-----+------+-----+-----+-----+------+-----+-----
    // ------+-----+------+-----+-----+-----+------+-----+-----
    // CHIP1 |MONO1|SELLR1|INVL1|INVR1|MONO2|SELLR2|INVL2|INVR2
    // ------+-----+------+-----+-----+-----+------+-----+-----
    // STEREO|  -  |   -  |  -  |  -  |  -  |  -   |  -  |  -  
    // ------+-----+------+-----+-----+-----+------+-----+-----
    // MONO  |  1  |   1  |  1  |  1  |  1  |  1   |  0  |  0  
    // --------------------------------------------------------
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
  // ------+-----+------+-----+-----+
  // CHIP0 |MONO |SELLR |INVL |INVR |
  // ------+-----+------+-----+-----+
  // STEREO|  0  |   0  |  0  |  0  |
  // ------+-----+------+-----+-----+
  // MONO  |  1  |   0  |  1  |  0  |
  // ------+-----+------+-----+-----+
  // ------+-----+------+-----+-----+
  // CHIP1 |MONO |SELLR |INVL |INVR |
  // ------+-----+------+-----+-----+
  // STEREO|  -  |   -  |  -  |  -  |
  // ------+-----+------+-----+-----+
  // MONO  |  1  |   1  |  0  |  0  |
  // --------------------------------
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
  bitWrite(ak449Chip0.Dsd1, 2, 1);
  bitWrite(ak449Chip1.Dsd1, 2, 1);

  // DSDSEL[1:0]: DSDデータストリームレートの選択
  // DSDSEL1
  b = bitRead(cpld.deviceConfig1, 3);
  bitWrite(ak449Chip0.Dsd2, 0, b);
  bitWrite(ak449Chip1.Dsd2, 0, b);
  // DSDSEL0
  b = bitRead(cpld.deviceConfig1, 2);
  bitWrite(ak449Chip0.Dsd1, 0, b);
  bitWrite(ak449Chip1.Dsd1, 0, b);
  
  // DSDD: DSD playback path control
  DSDD = bitRead(cpld.deviceConfig1, 1);
  bitWrite(ak449Chip0.Dsd1, 1, DSDD);
  bitWrite(ak449Chip1.Dsd1, 1, DSDD);

  // GC[1:0]: Gain Control
  GC1 = bitRead(cpld.deviceConfig0, 1);
  bitWrite(ak449Chip0.Ctrl5, 2, GC1);
  bitWrite(ak449Chip1.Ctrl5, 2, GC1);
  GC0 = bitRead(cpld.deviceConfig0, 0);
  bitWrite(ak449Chip0.Ctrl5, 1, GC0);
  bitWrite(ak449Chip1.Ctrl5, 1, GC0);

  // SYNCE: クロックの同期化機能を有効）
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
  
  // ADPE: PCM/DSD再生モード自動切り替え
  // ADPT[1:0]: 切替時のモード判定を開始するまでの時間設定
  ak449Chip0.Ctrl8 = 0xE0;  // 自動切り替えを有効にし、判定までの時間は1024/fs+18/fs
  ak449Chip1.Ctrl8 = 0xE0;

  // Volume
  
}
