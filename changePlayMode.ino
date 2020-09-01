/*************************************************************
  再生モードの変更

  
 *************************************************************/
 void changePlayMode(){
  int DSD = digitalRead(DP);
  int DSDSEL = cpld.deviceConfig1 & 0x0C;

  /*  */
  if (blynkMuteButton == 1) {
    bitWrite(ak449Chip0.Ctrl2, 0, 1); 
    i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute ON
    digitalWrite(pwLED,LOW);
  } else {
    bitWrite(ak449Chip0.Ctrl2, 0, 0); 
    i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);  // Soft mute OFF   
   digitalWrite(pwLED,HIGH);
  }
  
  /* DSDデータストリームレートの設定 */
  // DSDデータストリームレートの設定がAutoの場合
  if (DSDSEL == 0x0C) {
    // DSDデータストリームレートがDSD512の場合
    if (DSD64 == 0x42) {
      // DSDSEL[1:0]=(11)
      bitWrite(ak449Chip0.Dsd1, 0, 1);
      bitWrite(ak449Chip0.Dsd2, 0, 1);
      bitWrite(ak449Chip1.Dsd1, 0, 1);
      bitWrite(ak449Chip1.Dsd2, 0, 1);
    // DSDデータストリームレートがDSD256の場合
    } else if (DSD64 == 0x40) {
      // DSDSEL[1;0]=(10)
      bitWrite(ak449Chip0.Dsd1, 0, 0);
      bitWrite(ak449Chip0.Dsd2, 0, 1);
      bitWrite(ak449Chip1.Dsd1, 0, 0);
      bitWrite(ak449Chip1.Dsd2, 0, 1);
    // DSDデータストリームレートがDSD128の場合
    } else if (DSD64 == 0x02) {
      // DSDSEL[1:0]=(01)
      bitWrite(ak449Chip0.Dsd1, 0, 1);
      bitWrite(ak449Chip0.Dsd2, 0, 0);
      bitWrite(ak449Chip1.Dsd1, 0, 1);
      bitWrite(ak449Chip1.Dsd2, 0, 0);
    // DSDデータストリームレートがDSD64の場合
    } else if (DSD64 == 0x00) {
      // DSDSEL[1:0]=(00)
      bitWrite(ak449Chip0.Dsd1, 0, 0);
      bitWrite(ak449Chip0.Dsd2, 0, 0);
      bitWrite(ak449Chip1.Dsd1, 0, 0);
      bitWrite(ak449Chip1.Dsd2, 0, 0);
    }
  // DSDデータストリームレートの設定がDSD64固定
  } else if (DSDSEL == 0x00) {
    bitWrite(ak449Chip0.Dsd1, 0, 0);
    bitWrite(ak449Chip0.Dsd2, 0, 0);
    bitWrite(ak449Chip1.Dsd1, 0, 0);
    bitWrite(ak449Chip1.Dsd2, 0, 0);
  // DSDデータストリームレートの設定がDSD128固定
  } else if (DSDSEL == 0x04) {
    bitWrite(ak449Chip0.Dsd1, 0, 1);
    bitWrite(ak449Chip0.Dsd2, 0, 0);
    bitWrite(ak449Chip1.Dsd1, 0, 1);
    bitWrite(ak449Chip1.Dsd2, 0, 0);
  // DSDデータストリームレートの設定がDSD256固定
  } else if (DSDSEL == 0x08) {
    bitWrite(ak449Chip0.Dsd1, 0, 0);
    bitWrite(ak449Chip0.Dsd2, 0, 1);
    bitWrite(ak449Chip1.Dsd1, 0, 0);
    bitWrite(ak449Chip1.Dsd2, 0, 1);
  }
  // AK4499のDSD1およびDSD2レジスタに設定値を書き込む
  i2cWrite(AK449_Chip0, 0x06, ak449Chip0.Dsd1);
  i2cWrite(AK449_Chip0, 0x09, ak449Chip0.Dsd2);
  if (mono == 0x80) {
    i2cWrite(AK449_Chip1, 0x06, ak449Chip0.Dsd1);
    i2cWrite(AK449_Chip1, 0x09, ak449Chip0.Dsd2);
  }

  /* 再生モードの変更 */
  // 再生モードがDSDからPCMに変更した場合
  if ( (prevMode == 1) && (DSD == 0)  ) {
    // DPビット(DSD/PCMモード選択)に'0'(PCMモード)を設定
    bitWrite(ak449Chip0.Ctrl3, 7, 0);
    i2cWrite(AK449_Chip0, 0x02, ak449Chip0.Ctrl3);
    delay(0);
    // RSTNビット(内部タイミングリセット)に'0'を設定しタイミングリセットする
    bitWrite(ak449Chip0.Ctrl1, 0, 0);
    i2cWrite(AK449_Chip0, 0x00, ak449Chip0.Ctrl1);
    // 180ミリ秒ウェイト処理
    delay(180);
    // RSTNビットに'1'を設定しリセットを解除する
    bitWrite(ak449Chip0.Ctrl1, 0, 1);
    i2cWrite(AK449_Chip0, 0x00, ak449Chip0.Ctrl1);
    // デュアルモノモードの場合の処理
    if (mono == 0x80) {
      // DPビットに'0'を設定
      bitWrite(ak449Chip1.Ctrl3, 7, 0);
      i2cWrite(AK449_Chip1, 0x02, ak449Chip1.Ctrl3);
      delay(0);
      // RSTNビットに'0'を設定し内部タイミングをリセット
      bitWrite(ak449Chip1.Ctrl1, 0, 0);
      i2cWrite(AK449_Chip1, 0x00, ak449Chip1.Ctrl1);
      // 180ミリ秒のウェイト処理
      delay(180);
      // RSTNビットに'1'を設定しリセットを解除
      bitWrite(ak449Chip1.Ctrl1, 0, 1);
      i2cWrite(AK449_Chip1, 0x00, ak449Chip1.Ctrl1);
    }
    // PCMモードに変わったことによる内部タイミングリセットの発生をシリアルモニタに出力
    Serial.println("Internal Timing Reset by PCM");
  // 再生モードがPCMからDSDに変化した場合
  } else if ( (prevMode == 0) && (DSD == 1) ) {
    // DPビットに'1'(DSDモード)を設定
    bitWrite(ak449Chip0.Ctrl3, 7, 1);
    i2cWrite(AK449_Chip0, 0x02, ak449Chip0.Ctrl3);
    delay(0);
    // RSTNビットに'0'を設定
    bitWrite(ak449Chip0.Ctrl1, 0, 0);
    i2cWrite(AK449_Chip0, 0x00, ak449Chip0.Ctrl1); 
    // 180ミリ秒のウェイト処理
    delay(180);
    // RSTNビットに'1'を設定しリセット解除
    bitWrite(ak449Chip0.Ctrl1, 0, 1);
    i2cWrite(AK449_Chip0, 0x00, ak449Chip0.Ctrl1);
    // デュアルモノモードの場合の処理
    if (mono == 0x80) {
      // DPビットに'0'を設定      
      bitWrite(ak449Chip1.Ctrl3, 7, 1);
      i2cWrite(AK449_Chip1, 0x02, ak449Chip1.Ctrl3);
      delay(0);
      // RSTNビットに'0'を設定し内部タイミングをリセット
      bitWrite(ak449Chip1.Ctrl1, 0, 0);
      i2cWrite(AK449_Chip1, 0x00, ak449Chip1.Ctrl1);
      // 180ミリ秒のウェイト処理
      delay(180);
      bitWrite(ak449Chip1.Ctrl1, 0, 1);
      i2cWrite(AK449_Chip1, 0x00, ak449Chip1.Ctrl1);
    }
    // DSDモードに変わったことによる内部タイミングのリセットの発生をシリアルモニタに出力
    Serial.println("Internal Timing Reset by DSD");
  }
  prevMode = DSD;
}
