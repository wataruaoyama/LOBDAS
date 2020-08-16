#include <Wire.h>
#include "SO2002A_I2C.h"
#include <Preferences.h>
#include "ak449.h"

#define SDA 21
#define SCL 22

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "your Auth Token";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "your SSID";
char pass[] = "your passward";


int ledbit;

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
  
  attachInterrupt(DP, playMode, CHANGE);
  
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
  }
  preferences.end();

  volumeCounter = volumeValue;
  //
  Serial.begin(115200);
  Wire.begin(SDA, SCL);
  Wire.setClock(400000);
  delay(1000);  // Wait 1 second for reset CPLD
  initRegister();
  initDisplay();
//  readAK449Chip1Register();  

  Blynk.begin(auth, ssid, pass);

  digitalWrite(pwLED,HIGH);
  
//  ledbit = 1;
//  delay(2000);
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
//    preferences.end();
  }
  preferences.end();

  messageOut();
  changeFilter();
//  playMode();   // Use external interrupt
  changePlayMode();
//  if (ledbit == 1) {
//    digitalWrite(pwLED,LOW);
//  } else if (ledbit == 0) {
//    digitalWrite(pwLED,HIGH);
//  }
//  ledbit = not ledbit;
//  serialDebug();
//  readDeviceName();
  
  delay(10);
}

BLYNK_WRITE(V0){
  volumeCounter = param[0].asInt();
}

BLYNK_WRITE(V1){
  int blynkButton = param[0].asInt();
  if ( blynkButton == 1) {
    cnt++;
    Serial.print("blynkButton = ");
    Serial.println(blynkButton,DEC);
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
  buttonState = state;
  i2cWrite(AK449_Chip0, 0x01, ak449Chip0.Ctrl2);
  i2cWrite(AK449_Chip0, 0x02, ak449Chip0.Ctrl3);
  i2cWrite(AK449_Chip0, 0x05, ak449Chip0.Ctrl4);
  if ( mono == 0x80 ) {
    i2cWrite(AK449_Chip1, 0x01, ak449Chip1.Ctrl2);
    i2cWrite(AK449_Chip1, 0x02, ak449Chip1.Ctrl3);
    i2cWrite(AK449_Chip1, 0x05, ak449Chip1.Ctrl4);    
  }
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

void serialDebug() {
  Serial.print("Ctrl1 = 0x");
  Serial.println(ak449Chip0.Ctrl1, HEX);
  Serial.print("Ctrl2 = 0x");
  Serial.println(ak449Chip0.Ctrl2, HEX);
  Serial.print("Ctrl3 = 0x");
  Serial.println(ak449Chip0.Ctrl3, HEX);
  Serial.print("L1chATT = 0x");
  Serial.println(ak449Chip0.L1chATT, HEX);
  Serial.print("R1chATT = 0x");
  Serial.println(ak449Chip0.R1chATT, HEX);
  Serial.print("Ctrl4 = 0x");
  Serial.println(ak449Chip0.Ctrl4, HEX);
  Serial.print("Dsd1 = 0x");
  Serial.println(ak449Chip0.Dsd1, HEX);
  Serial.print("Ctrl5 = 0x");
  Serial.println(ak449Chip0.Ctrl5, HEX);
  Serial.print("Dsd2 = 0x");
  Serial.println(ak449Chip0.Dsd2, HEX);
  Serial.print("Ctrl6 = 0x");
  Serial.println(ak449Chip0.Ctrl6, HEX);
  Serial.print("Ctrl7 = 0x");
  Serial.println(ak449Chip0.Ctrl7, HEX);
  Serial.print("L2chATT = 0x");
  Serial.println(ak449Chip0.L2chATT, HEX);
  Serial.print("R2chATT = 0x");
  Serial.println(ak449Chip0.R2chATT, HEX);
  Serial.print("Ctrl8 = 0x");
  Serial.println(ak449Chip0.Ctrl8, HEX);

  if ( mono == 0x80 ) {
    Serial.print("Ctrl1 = 0x");
    Serial.println(ak449Chip1.Ctrl1, HEX);
    Serial.print("Ctrl2 = 0x");
    Serial.println(ak449Chip1.Ctrl2, HEX);
    Serial.print("Ctrl3 = 0x");
    Serial.println(ak449Chip1.Ctrl3, HEX);
    Serial.print("L1chATT = 0x");
    Serial.println(ak449Chip1.L1chATT, HEX);
    Serial.print("R1chATT = 0x");
    Serial.println(ak449Chip1.R1chATT, HEX);
    Serial.print("Ctrl4 = 0x");
    Serial.println(ak449Chip1.Ctrl4, HEX);
    Serial.print("Dsd1 = 0x");
    Serial.println(ak449Chip1.Dsd1, HEX);
    Serial.print("Ctrl5 = 0x");
    Serial.println(ak449Chip1.Ctrl5, HEX);
    Serial.print("Dsd2 = 0x");
    Serial.println(ak449Chip1.Dsd2, HEX);
    Serial.print("Ctrl6 = 0x");
    Serial.println(ak449Chip1.Ctrl6, HEX);
    Serial.print("Ctrl7 = 0x");
    Serial.println(ak449Chip1.Ctrl7, HEX);
    Serial.print("L2chATT = 0x");
    Serial.println(ak449Chip1.L2chATT, HEX);
    Serial.print("R2chATT = 0x");
    Serial.println(ak449Chip1.R2chATT, HEX);
    Serial.print("Ctrl8 = 0x");
    Serial.println(ak449Chip1.Ctrl8, HEX);
  }
  Serial.print("hwConfig = 0x");
  Serial.println(cpld.hwConfig, HEX);
  Serial.print("deviceConfig0 = 0x");
  Serial.println(cpld.deviceConfig0, HEX);
  Serial.print("deviceConfig1 = 0x");
  Serial.println(cpld.deviceConfig1, HEX);   
}

void readAK449Chip1Register() {
  byte Ctrl1 = i2cRead(AK449_Chip0, 0x00);
  Serial.print("Ctrl1 of AK4499_Chip0 = 0x");
  Serial.println(Ctrl1, HEX);
  byte Ctrl2 = i2cRead(AK449_Chip0, 0x01);
  Serial.print("Ctrl2 of AK4499_Chip0 = 0x");
  Serial.println(Ctrl2, HEX);
  byte Ctrl3 = i2cRead(AK449_Chip0, 0x02);
  Serial.print("Ctrl3 of AK4499_Chip0 = 0x");
  Serial.println(Ctrl3, HEX);
  byte Ctrl4 = i2cRead(AK449_Chip0, 0x05);
  Serial.print("Ctrl4 of AK4499_Chip0 = 0x");
  Serial.println(Ctrl4, HEX);
  byte Ctrl5 = i2cRead(AK449_Chip0, 0x07);
  Serial.print("Ctrl5 of AK4499_Chip0 = 0x");
  Serial.println(Ctrl5, HEX);
  byte Ctrl6 = i2cRead(AK449_Chip0, 0x0A);
  Serial.print("Ctrl6 of AK4499_Chip0 = 0x");
  Serial.println(Ctrl6, HEX);
  byte Ctrl7 = i2cRead(AK449_Chip0, 0x0B);
  Serial.print("Ctrl7 of AK4499_Chip0 = 0x");
  Serial.println(Ctrl7, HEX);
  byte Ctrl8 = i2cRead(AK449_Chip0, 0x15);
  Serial.print("Ctrl8 of AK4499_Chip0 = 0x");
  Serial.println(Ctrl8, HEX);
  byte L1chATT = i2cRead(AK449_Chip0, 0x03);
  Serial.print("L1chATT of AK4499_Chip0 = 0x");
  Serial.println(L1chATT, HEX);  
}

void readDeviceName() {
  Serial.print("DeviceName = 0x");
  Serial.println(deviceName);
}
