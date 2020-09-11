/*************************************************************
  LOBDAS(LOgic Board for Digital Audio System
  ********************************************
  
  ロジック基板説明書:
   http://linuxcom.info/netshop/LOBDAS_manual.pdf

  LOBDASはAKMのプレミアムDAC AK449xシリーズと組み合わせてデジタル
  オーディオシステムを構成する．

  スマホとの無線接続（BLE、WiFi）を容易にするためにBlynkアプリに対応する．
  
 *************************************************************/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BlynkSimpleEsp32_BLE.h>

#include <Wire.h>
#include "SO2002A_I2C.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Preferences.h>
#include "ak449.h"

#define SDA 21
#define SCL 22

// 126x64pixel SSD1306 OLED
Adafruit_SSD1306 display(128, 64, &Wire, -1);

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Your Auth Token";

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "SSID;
//char pass[] = "PASSWORD";

//BlynkのLCDのバーチャルピンは 3 
WidgetLCD lcd(3);

// SO2002Aのスレーブアドレスは0x3D
SO2002A_I2C oled(0x3D);

void setup() {
  // put your setup code here, to run once:
  delay(10);

  pinMode(upSwitch,INPUT);
  pinMode(downSwitch,INPUT);
  pinMode(filterSwitch,INPUT);
  pinMode(inputSwitch,INPUT);
  pinMode(pwLED, OUTPUT);
  pinMode(DP, INPUT);

  // SSD1306のスレーブアドレスは0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Adafuitのロゴ表示データを消去
  display.clearDisplay();

  // Setup timer interrupt
  // Timer: interrupt time and event setting. 
  timer1 = timerBegin(0, 80, true);
  //timer2 = timerBegin(1, 80, true);
  timer3 = timerBegin(2, 80, true);
  timer4 = timerBegin(3, 80, true);
    
  // Attach onTimer function.
  timerAttachInterrupt(timer1, &onTimer1, true);
  //timerAttachInterrupt(timer2, &onTimer2, true);
  timerAttachInterrupt(timer3, &onTimer3, true);
  timerAttachInterrupt(timer4, &onTimer4, true);
  
  // Set alarm to call onTimer function every second (value in microseconds).
  timerAlarmWrite(timer1, 150000, true); // 150ms
  //timerAlarmWrite(timer2, 150000, true); // 150ms
  timerAlarmWrite(timer3, 200000, true); // 200ms
  timerAlarmWrite(timer4, 200000, true); // 200ms
  
  // Start an alarm
  timerAlarmEnable(timer1);
  //timerAlarmEnable(timer2);
  timerAlarmEnable(timer3);
  timerAlarmEnable(timer4);
  
  // NVRAM setting
  preferences.begin("my-app", false);
  volumeValue = preferences.getInt("value", 255);
  preferences.end();

  if (volumeCounter != volumeValue ) {
    preferences.putInt("value", volumeCounter);
    preferences.end();
  }

  volumeCounter = volumeValue;
  //
  Serial.begin(115200);
  Wire.begin(SDA, SCL);
  // SCLの周波数を400kHzに設定する
  Wire.setClock(400000);
  // Wait 1 second for reset CPLD
  delay(1000);
  initRegister();
  initDisplay();
//  Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth);
  // デバッグ用のLEDを点灯
  digitalWrite(pwLED,HIGH);
}

void loop() {
  Blynk.run();
  
  // put your main code here, to run repeatedly:
  
  // Timer interrupt process
  if (timeCounter1 > 0) {
    portENTER_CRITICAL(&timerMux);
    timeCounter1--;
    portEXIT_CRITICAL(&timerMux);
    volumeControl();
  }

  if (timeCounter3 > 0) {
    portENTER_CRITICAL(&timerMux);
    timeCounter3--;
    portEXIT_CRITICAL(&timerMux);
    changeFilter();
  }
  
  if (timeCounter4 > 0) {
    portENTER_CRITICAL(&timerMux);
    timeCounter4--;
    portEXIT_CRITICAL(&timerMux);
    inputSelection();
  }
  
  preferences.begin("my-app", false);
  if (volumeCounter != volumeValue ) {
    preferences.putInt("value", volumeCounter);
    preferences.end();
  }
  
  messageOut();
  blynkDisplayOut();
  changePlayMode();
  
  delay(10);
}

/* Blynk virtual pin 0 function. */
// Blynkのスライダーの値をATTレベルにする
BLYNK_WRITE(V0){
  volumeCounter = param[0].asInt();
  unsigned char volume = volumeCounter;
  volume = ~volume;
  volumeCounter = volume;
}

/* Blynk veirtual pin 1 function */
// デジタルフィルタ特性の切り替え
BLYNK_WRITE(V1){
  int blynkButton = param[0].asInt();
  if ( blynkButton == 1) {
    cnt++;
    if ( cnt == 1 ) {
      bitWrite(ak449Chip0.Ctrl3, 0, 0);
      bitWrite(ak449Chip0.Ctrl2, 5, 0);
      bitWrite(ak449Chip0.Ctrl4, 0, 0);
      if ( mono == 0x80) {
        bitWrite(ak449Chip1.Ctrl3, 0, 0);
        bitWrite(ak449Chip1.Ctrl2, 5, 0);
        bitWrite(ak449Chip1.Ctrl4, 0, 0);        
      }
    } else if (cnt == 2) {
      bitWrite(ak449Chip0.Ctrl3, 0, 1);
      if ( mono == 0x80) {
        bitWrite(ak449Chip1.Ctrl3, 0, 1);       
      }
    } else if ( cnt == 3 ) {
      bitWrite(ak449Chip0.Ctrl3, 0, 0);
      bitWrite(ak449Chip0.Ctrl2, 5, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl3, 0, 0);
        bitWrite(ak449Chip1.Ctrl2, 5, 1);        
      }
    } else if ( cnt == 4 ) {
      bitWrite(ak449Chip0.Ctrl3, 0, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl3, 0, 1);        
      }
    } else if ( cnt == 5 ) {
      bitWrite(ak449Chip0.Ctrl3, 0, 0);
      bitWrite(ak449Chip0.Ctrl2, 5, 0);
      bitWrite(ak449Chip0.Ctrl4, 0, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl3, 0, 0);
        bitWrite(ak449Chip1.Ctrl2, 5, 0);
        bitWrite(ak449Chip1.Ctrl4, 0, 1);        
      }
    } else if ( cnt == 6 ) {
      bitWrite(ak449Chip0.Ctrl2, 5, 1);
      if ( mono == 0x80 ) {
        bitWrite(ak449Chip1.Ctrl2, 5, 1);
      }
      cnt = 0;
    }
  }
  i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);
  i2cWrite(AK449_Chip0, 0x02, ak449Chip0.Ctrl3);
  i2cWrite(AK449_Chip0, 0x05, ak449Chip0.Ctrl4);
  if ( mono == 0x80 ) {
    i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);
    i2cWrite(AK449_Chip1, 0x02, ak449Chip1.Ctrl3);
    i2cWrite(AK449_Chip1, 0x05, ak449Chip1.Ctrl4);    
  }
}

/* Blynk virtual pin 2 function */
// 入力ソースの切り替え
BLYNK_WRITE(V2){
  int blynkInsel = param.asInt();
    if ( blynkInsel == 1) {
    count++;
    if ( count == 1 ) {      
      i2cWrite(CPLD_ADR, 0x00, 0x00);     // Change Input to USB
      Serial.println("USB INPUT Selected");
    } else if (count == 2) {
      i2cWrite(CPLD_ADR, 0x00, 0x08);      // Change Inut to RJ45
      Serial.println("RJ45 INPUT Seleted");
    } else if (count == 3) {
      count = 0;
      Serial.println("XH INPUT Selected");
    }
  }
}

/* Blynk virtual pin 4 function */
// スマホのLCDの表示モードの切り替え
BLYNK_WRITE(V4){
  blynkModeButton = param[0].asInt();
}

/* Blynk virtual pin 6 function */
BLYNK_WRITE(V6){
  blynkMuteButton = param[0].asInt();
}

byte i2cRead(byte sladr, byte regadr){
  Wire.beginTransmission(sladr);
  Wire.write(regadr);
  Wire.endTransmission();
  Wire.requestFrom(sladr, 1);
  return Wire.read();
}

byte i2cWrite(byte sladr, byte regadr, byte wdata){
  Wire.beginTransmission(sladr);
  Wire.write(regadr);
  Wire.write(wdata);
  Wire.endTransmission();
}
