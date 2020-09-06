/*************************************************************
  ボリュームカウンタ
  ***************
  
  タイマー割り込み発生時に呼ばれる．
  ボリュームカウンタとしているが、実際にはATTカウンタである．
  volumeCounterが255でATT量は0dB,0で127.5dBとなる．
 *************************************************************/
void volumeControl() {
  boolean Up,Down;
  Up = digitalRead(upSwitch);
  Down = digitalRead(downSwitch);

  // UPスイッチが押された場合
  if ( !Up ) {
    // volumeCounterの値が255だったら
    if ( volumeCounter == 255 ){
      // volumeCounterの値を保持する
      volumeCounter = volumeCounter;
    }
    // volumeCounterの値が255以外であれば
    else {
      // volumeCounterの値をインクリメントする
      volumeCounter++;
    }
  }
  // Downスイッチが押された場合
  else if ( !Down ) {
    // volumeCounterが0になったら
    if ( volumeCounter == 0 ) {
      // volumeCounterの値を保持する
      volumeCounter = volumeCounter;
    }
    // volumeCounterの値が0以外であれば
    else {
      // volumeCounterをデクリメントする
      volumeCounter--;
    }    
  }
  // DACのATTレジスタにvolumeCounterの値を設定する
  i2cWrite(AK449_Chip0, 0x03, volumeCounter);
  i2cWrite(AK449_Chip0, 0x04, volumeCounter);
  i2cWrite(AK449_Chip0, 0x0C, volumeCounter);
  i2cWrite(AK449_Chip0, 0x0D, volumeCounter);  
  if ( mono == 0x80 ) {
    i2cWrite(AK449_Chip1, 0x03, volumeCounter);
    i2cWrite(AK449_Chip1, 0x04, volumeCounter);
    i2cWrite(AK449_Chip1, 0x0C, volumeCounter);
    i2cWrite(AK449_Chip1, 0x0D, volumeCounter);
  }
}
