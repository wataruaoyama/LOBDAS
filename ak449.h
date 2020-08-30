#define CPLD_ADR 0x52
#define AK449_Chip0 0x10
#define AK449_Chip1 0x12
#define upSwitch 4
#define downSwitch 39
#define filterSwitch 35
#define inputSwitch 34
#define pwLED 25
#define DP 5

Preferences preferences;
int volumeValue; 

int volumeCounter;
int state;
volatile int cnt = 3;
volatile int count = 1;
volatile int buttonState = HIGH;
volatile int inswState = HIGH;
volatile int blynkModeButton;
volatile int blynkMuteButton;

char Sharp[]            = "Sharp            ";
char Slow[]             = "Slow             ";
char ShortDelaySharp[]  = "Short Delay Sharp";
char ShortDelayShrp[]   = "Short Delay Shrp ";
char ShortDelaySlow[]   = "Short Delay Slow ";
char SuperSlow[]        = "Super Slow       ";
char LowDispersion[]    = "Low Dispersion   ";
char filterBlank[]      = "                 ";

char freq32[]           = "  32kHz  ";
char freq44[]           = "  44.1kHz";
char freq48[]           = "  48kHz  ";
char freq88[]           = "  88.2kHz";
char freq96[]           = "  96kHz  ";
char freq176[]          = " 176.4kHz";
char freq192[]          = " 192kHz  ";
char freq352[]          = " 352.8kHz";
char freq384[]          = " 384kHz  ";
char freqBlank[]        = "         ";
char freqDsd64[]        = " 2.8MHz  ";
char freqDsd128[]       = " 5.6MHz  ";
char freqDsd256[]       = " 11.2MHz ";
char freqDsd512[]       = " 22.4MHz ";

char audioIF0[]         = "16bit LSB";
char audioIF1[]         = "20bit LSB";
char audioIF2[]         = "24bit MSB";
char audioIF3[]         = "24bit I2S";
char audioIF4[]         = "24bit LSB";
char audioIF5[]         = "32bit LSB";
char audioIF6[]         = "32bit MSB";
char audioIF7[]         = "32bit I2S";

char dsdNormal[]        = "Normal";
char dsdBypass[]        = "Bypass";

char demOff[]           = "OFF";
char demOn[]            = "ON ";

char ak4499[]           = "AK4499";
char ak4493[]           = "AK4493";
char ak4495[]           = "AK4495S";
char ak4490[]           = "AK4490";
char others[]           = "Others";

char currentOut0[]      = "72m/72m/45mA";
char currentOut1[]      = "72m/45m/45mA";
char currentOut2[]      = "45m/45m/45mA";
char currentOut3[]      = "45m/45m/45mA";
char outputLevel0[]     = "5.6/5.6/5Vpp";
char outputLevel1[]     = "5.6/5/5Vpp";
char outputLevel2[]     = "5/5/5Vpp";
char outputLevel3[]     = "5/5/5Vpp";

volatile int DSDON;
volatile int FS;
volatile int DSD64;
volatile int mono;

int deviceName;
int DIF;
int DEM,DSDF;
bool DSDD;
bool GC0,GC1;

int prevMode = 1;


// Timer 
volatile int timeCounter1;
//volatile int timeCounter2;
volatile int timeCounter3;
volatile int timeCounter4;
hw_timer_t *timer1 = NULL;
//hw_timer_t *timer2 = NULL;
hw_timer_t *timer3 = NULL;
hw_timer_t *timer4 = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer1(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  timeCounter1++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

//void IRAM_ATTR onTimer2(){
//  // Increment the counter and set the time of ISR
//  portENTER_CRITICAL_ISR(&timerMux);
//  timeCounter2++;
//  portEXIT_CRITICAL_ISR(&timerMux);
//}

void IRAM_ATTR onTimer3(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  timeCounter3++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR onTimer4(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  timeCounter4++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

typedef struct registerMap1{
  byte Ctrl1;
  byte Ctrl2;
  byte Ctrl3;
  byte L1chATT;
  byte R1chATT;
  byte Ctrl4;
  byte Dsd1;
  byte Ctrl5;
  byte Rsv1;
  byte Dsd2;
  byte Ctrl6;
  byte Ctrl7;
  byte L2chATT;
  byte R2chATT;
  byte Rsv2;
  byte Rsv3;
  byte Rsv4;
  byte Rsv5;
  byte Rsv6;
  byte Rsv7;
  byte Rsv8;
  byte Ctrl8;
} registerMap1;

registerMap1 ak449Chip0;
registerMap1 ak449Chip1;
//registerMap1 ak449Chip2;
//registerMap1 ak449Chip3;

typedef struct registerMap2 {
  byte hwConfig;
  byte deviceConfig0;
  byte deviceConfig1;
  byte sampleRate;
} registerMap2;

registerMap2 cpld;
