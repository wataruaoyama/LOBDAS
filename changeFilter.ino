/*************************************************************
  デジタルフィルタ特性の変更
  **********************

  デジタルフィルタ変更スイッチが押される毎にデジタルフィルター特性を変更
  する．電源オン時にはショートディレイシャープになる．変更は以下のように

  ショートディレイシャープ -> ショートディレイスロー -> スーパースロー
  -> 低分散ショートディレイ -> シャープ -> スロー

  の順で変更される.

  設定表
  _______________________________________
  |SLOW| SD |SSLOW|    MODE
  +----+----+-----+----------------------
  | 0  | 0  |  0  | シャープ
  | 1  | 0  |  0  | スロー
  | 0  | 1  |  0  | ショートディレイシャープ
  | 1  | 1  |  0  | ショートディレイスロー
  | 0  | 0  |  1  | スーパースロー
  | 0  | 1  |  1  | 低分散ショートディレイ
  +--------------------------------------
 *************************************************************/
void changeFilter() {
  bool state = digitalRead(filterSwitch);

  if ( DSDON == 0x01 ) {
    oled.print(filterBlank);
  } 
  // PCMモードでスイッチがオフからオンに変化した時
  else if ( buttonState == HIGH && state == LOW) {
    // cntをインクリメント
    cnt++;
    // cntが'1'の場合、シャープに設定
    if ( cnt == 1 ) {
      // SLOW,SD,SSLOWビットを(000)にする
      bitWrite(ak449Chip0.Ctrl3, 0, 0); // SLOWビット
      bitWrite(ak449Chip0.Ctrl2, 5, 0); // SDビット
      bitWrite(ak449Chip0.Ctrl4, 0, 0); // SSLOWビット
      if ( mono == 0x80) {
        bitWrite(ak449Chip1.Ctrl3, 0, 0);
        bitWrite(ak449Chip1.Ctrl2, 5, 0);
        bitWrite(ak449Chip1.Ctrl4, 0, 0);        
      }
    }
    // cntが'2'の場合、スロー設定
    else if (cnt == 2) {
      // SLOW,SD,SSLOWビットを(100)にする
      bitWrite(ak449Chip0.Ctrl3, 0, 1);
      if ( mono == 0x80) {
        bitWrite(ak449Chip1.Ctrl3, 0, 1);       
      }
    }
    // cntが'3'の場合、ショートディレイシャープ設定
    else if ( cnt == 3 ) {
      // SLOW,SD,SSLOWビットを(010)にする
      bitWrite(ak449Chip0.Ctrl3, 0, 0);
      bitWrite(ak449Chip0.Ctrl2, 5, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl3, 0, 0);
        bitWrite(ak449Chip1.Ctrl2, 5, 1);        
      }
    }
    // cntが'4'の場合、ショートディレイスロー設定
    else if ( cnt == 4 ) {
      // SLOW,SD,SSLOWビットを(110)にする
      bitWrite(ak449Chip0.Ctrl3, 0, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl3, 0, 1);        
      }
    }
    // cntが'5'の場合、スーパースロー設定
    else if ( cnt == 5 ) {
      // SLOW,SD,SSLOWビットを(001)にする
      bitWrite(ak449Chip0.Ctrl3, 0, 0);
      bitWrite(ak449Chip0.Ctrl2, 5, 0);
      bitWrite(ak449Chip0.Ctrl4, 0, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl3, 0, 0);
        bitWrite(ak449Chip1.Ctrl2, 5, 0);
        bitWrite(ak449Chip1.Ctrl4, 0, 1);        
      }
    }
    // cntが'6'の場合、低分散ショートディレイ設定
    else if ( cnt == 6 ) {
      // SLOW,SD,SSLOWビットを(011)にする
      bitWrite(ak449Chip0.Ctrl2, 5, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl2, 5, 1);
      }
      cnt = 0;
    }
  }
  buttonState = state;
  // 設定内容をDACに書き込む
  i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);
  i2cWrite(AK449_Chip0, 0x02, ak449Chip0.Ctrl3);
  i2cWrite(AK449_Chip0, 0x05, ak449Chip0.Ctrl4);
  if ( mono == 0x80 ) {
    i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);
    i2cWrite(AK449_Chip1, 0x02, ak449Chip1.Ctrl3);
    i2cWrite(AK449_Chip1, 0x05, ak449Chip1.Ctrl4);    
  }
}
