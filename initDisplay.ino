/*************************************************************
  システム立ち上がり時のディスプレイ表示
  ********************************
  
  システムの電源オン時にシステム構成、設定内容の表示を行う．
  表示内容は
  1.使用DACと構成（ステレオ／デュアルモノ）
  2.Audio InterfaceフォーマットとDSD playback path
  3.ゲイン設定とディエンファシスおよびDSDフィルタのカットオフ設定
  で、各3秒間表示する．
  
 *************************************************************/
void initDisplay() {
  
  oled.begin(20, 2);
  oled.clear();
  oled.setCursor(0,0);
  /* 最初の3秒間の表示 */
  // DACがAK4499
  if ( deviceName == 0x07 ) {
    // AK4499を表示
    oled.print(ak4499);
  }
  // DACがAK4493
  else if ( deviceName == 0x06 ) {
    // AK4493を表示
    oled.print(ak4493);
  }
  // DACがAK4495
  else if ( deviceName == 0x05 ) {
    // AK4495を表示
    oled.print(ak4495);
  }
  // DACがAK4490
  else if ( deviceName == 0x04 ) {
    // AK4490を表示
    oled.print(ak4490);
  }
  // それ以外のDAC
  else {
    // Othersを表示
   oled.print(others);
  }
  
  oled.setCursor(6,0);
  // ステレオモードの設定
  if ( mono == 0xC0 ) {
    oled.print(" Stereo");
  }
  // デュアル・モノモード設定の場合
  else if ( mono == 0x80 ) {
    oled.print(" Dual Mono");
  }
  oled.setCursor(0,1);
  oled.print("by LINUXCOM");
  delay(3000);  
  oled.clear();

  /* 2回目の3秒間の表示 */
  // 設定されたオーディオインターフェースフォーマットの表示
  oled.setCursor(0,0);
  oled.print("DIF:");
  oled.setCursor(4,0);
  // DIF[2:0]=(111)の場合
  if ( DIF == 0x38 ) {
    // 32bit I2Sを表示
    oled.print(audioIF7);
  }
  // DIF[2:0]=(110)の場合
  else if ( DIF == 0x30 ) {
    // 32bit MSBを表示
    oled.print(audioIF6);
  }
  // DIF[2:0]=(101)の場合
  else if ( DIF == 0x28 ) {
    // 32bit LSBを表示
    oled.print(audioIF5);
  }
  // DIF[2:0]=(100)の場合
  else if ( DIF == 0x20 ) {
    // 24bit LSBを表示
    oled.print(audioIF4);
  }
  // DIF[2:0]=(011)の場合
  else if ( DIF == 0x18 ) {
    // 24bit I2Sを表示
    oled.print(audioIF3);
  }
  // DIF[2:0]=(010)の場合
  else if ( DIF == 0x10 ) {
    // 24bit MSBを表示
    oled.print(audioIF2);
  }
  // DIF[2:0]=(001)の場合
  else if ( DIF == 0x08 ) {
    // 20bit LSBを表示
    oled.print(audioIF1);
  }
  // DIF[2:0]=(000)の場合
  else if ( DIF == 0x00 ) {
    // 16bit LSBを表示
    oled.print(audioIF0);
  }
  // DSDモード時のPlayback Pathの設定状態を表示する
  oled.setCursor(0,1);
  oled.print("DSDD:");
  oled.setCursor(5,1);
  // 設定がVolume Bypassの場合
  if ( DSDD == HIGH ) {
    // Bypassを表示
    oled.print(dsdBypass);
  }
  // 設定がNormalの場合
  else {
    // Normalを表示
    oled.print(dsdNormal);
  }
  delay(3000);
  oled.clear();
  
  /* 3回目の3秒間の表示 */
  oled.setCursor(0,0);
  // DACがAK4499の場合
  if ( deviceName == 0x07 ) {
    // Current Output Level
    oled.print("COL:");
    oled.setCursor(4,0);
    // GC[1:0]=(11)の場合
    if (( GC1 == HIGH ) && ( GC0 == HIGH )) {
      // 45m/45m/45mAを表示
      oled.print(currentOut3);
    }
    // GC[1:0]=(10)の場合
    else if (( GC1 == HIGH ) && ( GC0 == LOW )) {
      // 45m/45m/45mAを表示
      oled.print(currentOut2);
    } 
    // GC[1:0]=(01)の場合
    else if (( GC1 == LOW ) && ( GC0 == HIGH )) {
      // 72m/45m/45mAを表示
      oled.print(currentOut1);
    }
    // GC[1:0]=(00)の場合
    else if (( GC1 == LOW ) && ( GC0 == LOW )) {
      // 72m/72m/45mAを表示
      oled.print(currentOut0);
    }
  }
  // DACがAK4490/95/59Sの場合
  else {
    // Voltage Output Level
    oled.print("VOL:");
    oled.setCursor(4,0);
    // GC[1:0]=(11)の場合
    if (( GC1 == HIGH ) && ( GC0 == HIGH )) {
      // 5/5/5Vppを表示
      oled.print(outputLevel3);
    }
    // GC[1:0]=(10)の場合
    else if (( GC1 == HIGH ) && ( GC0 == LOW )) {
      // 5/5/5Vppを表示
      oled.print(outputLevel2);
    }
    // GC[1:0]=(01)の場合
    else if (( GC1 == LOW ) && ( GC0 == HIGH )) {
      // 5.6/5/5Vpp
      oled.print(outputLevel1);
    }
    // GC[1:0]=(00)の場合
    else if (( GC1 == LOW ) && ( GC0 == LOW )) {
      // 5.6/5.6/5Vppを表示
      oled.print(outputLevel0);
    }
  }
  // ディエンファシスの設定内容
  oled.setCursor(0,1);
  oled.print("DEM:");
  oled.setCursor(4,1);
  // ディエンファシスがオフ
  if ( DEM == 0x10 ) {
    oled.print(demOff);
  }
  // ディエンファシスがオン
  else {
    oled.print(demOn);
  }
  // DSD64/DSD128の時のDSDフィルタのカットオフ設定
  oled.setCursor(8,1);
  oled.print("DSDF:");
  // 
  if ( DSDF == 0x01 ) {
    oled.print("Low");
  } else {
    oled.print("High");
  }
  delay(3000);
  oled.clear();
}
