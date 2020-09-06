/*************************************************************
  レジスタの初期化
  ***************
  
  CPLDとAK449xの内部レジスタを初期化する．
  
 *************************************************************/
void initRegister() {

//  ak449Chip0.Ctrl1 = 0x8F;            // 0x00
//  ak449Chip0.Ctrl2 = 0x2A;            // 0x01
//  ak449Chip0.Ctrl3 = 0x08;            // 0x02
//  ak449Chip0.L1chATT = volumeCounter; // 0x03
//  ak449Chip0.R1chATT = volumeCounter; // 0x04
//  ak449Chip0.Ctrl4 = 0x0A;            // 0x05
//  ak449Chip0.Dsd1 = 0x81;             // 0x06
//  ak449Chip0.Ctrl5 = 0x04;            // 0x07
//  ak449Chip0.Rsv1 = 0x00;             // 0x08
//  ak449Chip0.Dsd2 = 0x04;             // 0x09
//  ak449Chip0.Ctrl6 = 0x0D;            // 0x0A
//  ak449Chip0.Ctrl7 = 0x20;            // 0x0B
//  ak449Chip0.L2chATT = volumeCounter; // 0x0C
//  ak449Chip0.R2chATT = volumeCounter; // 0x0D
//  ak449Chip0.Rsv2 = 0x00;             // 0x0E
//  ak449Chip0.Rsv3 = 0x00;             // 0x0F
//  ak449Chip0.Rsv4 = 0x00;             // 0x10
//  ak449Chip0.Rsv5 = 0x00;             // 0x11
//  ak449Chip0.Rsv6 = 0x00;             // 0x12
//  ak449Chip0.Rsv7 = 0x00;             // 0x13
//  ak449Chip0.Rsv8 = 0x00;             // 0x14
//  ak449Chip0.Ctrl8 = 0x00;            // 0x15 

  i2cWrite(CPLD_ADR, 0x00, 0x00);
  initAK449();
  
  if ((deviceName == 0x07) or (deviceName == 0x06)) {
    i2cWrite(AK449_Chip0, 0x15, ak449Chip0.Ctrl8);
    if ( mono == 0x80 ) {
      i2cWrite(AK449_Chip1, 0x15, ak449Chip0.Ctrl8);
    }
  }
  
  Wire.beginTransmission(AK449_Chip0);
  Wire.write(0x00);
  Wire.write(ak449Chip0.Ctrl1);
  Wire.write(ak449Chip0.Ctrl2);
  Wire.write(ak449Chip0.Ctrl3);
  Wire.write(ak449Chip0.L1chATT);
  Wire.write(ak449Chip0.R1chATT);
  Wire.write(ak449Chip0.Ctrl4);
  Wire.write(ak449Chip0.Dsd1);
  Wire.write(ak449Chip0.Ctrl5);
  Wire.write(ak449Chip0.Rsv1);
  Wire.write(ak449Chip0.Dsd2);
  if ((deviceName == 0x07) or (deviceName == 0x06)) {
    Wire.write(ak449Chip0.Ctrl6);
    Wire.write(ak449Chip0.Ctrl7);
    Wire.write(ak449Chip0.L2chATT);
    Wire.write(ak449Chip0.R2chATT);
  }
  Wire.endTransmission();

  if ( mono == 0x80 ) {
    Wire.beginTransmission(AK449_Chip1);
    Wire.write(0x00);
    Wire.write(ak449Chip1.Ctrl1);
    Wire.write(ak449Chip1.Ctrl2);
    Wire.write(ak449Chip1.Ctrl3);
    Wire.write(ak449Chip1.L1chATT);
    Wire.write(ak449Chip1.R1chATT);
    Wire.write(ak449Chip1.Ctrl4);
    Wire.write(ak449Chip1.Dsd1);
    Wire.write(ak449Chip1.Ctrl5);
    Wire.write(ak449Chip1.Rsv1);
    Wire.write(ak449Chip1.Dsd2);
    if ((deviceName == 0x07) or (deviceName == 0x06)) {
      Wire.write(ak449Chip1.Ctrl6);
      Wire.write(ak449Chip1.Ctrl7);
      Wire.write(ak449Chip1.L2chATT);
      Wire.write(ak449Chip1.R2chATT);
    }
    Wire.endTransmission();
  }
}
