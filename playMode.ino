void playMode(){
  int DSD = digitalRead(DP);

  if ( DSD == 0  ) {   // PCM mode
    bitWrite(ak449Chip0.Ctrl3, 7, 0);
  } else if ( DSD == 1 ) {   // DSD mode
    bitWrite(ak449Chip0.Ctrl3, 7, 1);
  }
}
