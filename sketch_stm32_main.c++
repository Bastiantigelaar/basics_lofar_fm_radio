// this represent the sketch from the stm32 chip without supporting the nucleo f303k8
// this means that the pinout will vary from the other main.cpp hence therefor this code will be named sketch_stm32_main.cpp
#include <Arduino.h>
#include <Ticker.h>

// FM CHIP
#include <SI4735.h>
#include <control.h>
#include <FM_chip.h>

// DISPLAY
#include <TFT.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// the low power library.
#include "STM32LowPower.h"


// fm chip declaration
SI4735 si4735;

// display [tft] declaration

// Adafruit_ILI9341 tft = Adafruit_ILI9341(PB_12, PB_11,PB_10);

Adafruit_ILI9341 tft = Adafruit_ILI9341(PB_12, PB_11, PB_15, PB_13, PB_10, PB_14);

volatile int i = 0;
//  functions prototypes
void frequency_up_pressed();
void frequency_down_pressed();
void volume_up_pressed();
void volume_down_pressed();

// void showHelp();

// DISPLAY FUNCTION
void main_display();
void second_display();
void volume_display();
void frequency_display();
void SNR_display();
void audio_display();
void signal_display();
void checkRDS();
void showProgramInfo();
void showRDSStation();
void clearRdsBuffer();
void showRDSTime();
void update_program_info();
bool get_menu();

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t currentVolume;
uint8_t previousVolume;
bool currentmenustate;
bool previousmenustate;
#define SSB 1

#define STORE_TIME 10000 // Time of inactivity to make the current receiver status writable (10s / 10000 milliseconds).

#define CLEAR_BUFFER(x) (x[0] = '\0');

const uint8_t app_id = 35; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;
long storeTime = millis();

bool bfoOn = false;
bool ssbLoaded = false;
bool fmStereo = true;

bool menu_toggle = true;
void toggle_menu();

// volatile float current = 100e6;

// buttons frequency up and down

int frequency_up_button = PA_4;
int frequency_down_button = PA_5;

int toggle_menu_button = PA_3;

// buttons volume up and down
int volume_up_button = PA_2;
int volume_down_button = PA_1;

// low power pin amplifier
int Shutdown_amplifier = PA_8;

// control of FM switch
int V1 = PA_15;
int V2 = PA_14;


int batterij_inlezen  = PB_0;

// buttons band up and down

// int band_up_button = 16;
// int band_down_button = 17;

// button toggle stereo and mono // is this possible ?? nope
// int toggle_stereo_mono = 29;

// RESET PIN FOR FM RADIO
#define RESET_PIN PA_13

// FM function enabaled [1 == AM]
#define FM_FUNCTION 0

// RDS begin
char *programInfo;
char *stationName;
char *rdsTime;
char *stationInfo;

char bufferStatioName[40];
char bufferRdsMsg[65];
char bufferRdsTime[32];

long delayStationName = millis();
long delayProgramInfo = millis();
long delatRdsTime = millis();

uint16_t timeToShowProgram = 1000;

bool bShowStationName = true;
int progInfoIdx = 0;

Ticker ticker_rds(checkRDS, 100);
/**
  showProgramInfo - Shows the Program Information
*/
void showProgramInfo()
{
  char txtAux[35];
  if (programInfo == NULL || (millis() - delayProgramInfo) < timeToShowProgram)
    return;

  timeToShowProgram = (progInfoIdx == 0) ? 1500 : 300;
  programInfo[60] = '\0'; // Removes unwanted characters 
  si4735.removeUnwantedChar(programInfo, 60);
  si4735.removeUnwantedChar(bufferRdsMsg, sizeof(txtAux));
  strncpy(txtAux, &programInfo[progInfoIdx], sizeof(txtAux));
  txtAux[sizeof(txtAux) - 1] = '\0';
  progInfoIdx++;
  if (progInfoIdx > 30)
    progInfoIdx = 0;
  si4735.removeUnwantedChar(txtAux, sizeof(txtAux));
  delayProgramInfo = millis();
}

void showRDSStation()
{
  if (stationName == NULL || (millis() - delayStationName) < 3000)
    return;
  si4735.removeUnwantedChar(stationName, 9);
  si4735.removeUnwantedChar(bufferStatioName, 9);
  delayStationName = millis();
}

void showRDSTime()
{
  if (rdsTime == NULL || (millis() - delatRdsTime) < 55000)
    return;
  delatRdsTime = millis();
}

void clearRdsBuffer()
{
  memset(bufferStatioName, 0, sizeof(bufferStatioName));
  memset(bufferRdsMsg, 0, sizeof(bufferRdsMsg));
  memset(bufferRdsTime, 0, sizeof(bufferRdsTime));
  si4735.RdsInit();
  programInfo = stationName = rdsTime = NULL;
}

void checkRDS()
{
  si4735.getRdsStatus();
  if (si4735.getRdsReceived())
  {
    Serial.print("ik ontvang iets");
    if (si4735.getRdsSync() && si4735.getNumRdsFifoUsed() > 0)
    {

      programInfo = si4735.getRdsProgramInformation();
      stationName = si4735.getRdsStationName();
      rdsTime = si4735.getRdsTime();
      stationInfo = si4735.getRdsStationInformation();

      showProgramInfo();
      showRDSStation();
      showRDSTime();
      update_menu();
    }
  }
  else
  {
    Serial.print("no RDS received. !! ");
  }
}
// RDS end

void setup()
{

  // defines and waits for serial com port
  Serial.begin(115200);
  //  while (!Serial)
  //  ;
  delay(20);

  Serial.print("IK STA AAN");
  // INDICATES THE PIN MODE

  // inilasation of lowpower

  // timer/ticker

  // setup of RF_SWITCH
  // |V1|V2|
  // |1 |0| -> PASSIEVE ANTENNE
  // |0 |1| -> LBA

  // default --> passieve antenne !
  pinMode(V1, OUTPUT);
  pinMode(V2, OUTPUT);
  delay(20);
  digitalWrite(V1, HIGH);
  digitalWrite(V2, LOW);

  // setup of the ANALOG IN
   // batterij_inlezen wordt gezet op een analoge input met 2^12 = 4096 kwantanserings niveau
  pinMode(batterij_inlezen, INPUT);
  analogReadResolution(12);

  // turns  amplifier on

  pinMode(Shutdown_amplifier, OUTPUT);
  digitalWrite(Shutdown_amplifier, HIGH);
  // reset pin
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);

  // check status FM chip
  // si4735.getStatus();
  // config_i2c_adress();
  delay(500);

  // Look for the Si47XX I2C bus address

  int16_t si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
  if (si4735Addr == 0)
  {
    Serial.println("Si473X not found!");
    Serial.flush();
    while (1)
      ;
  }
  else
  {
    Serial.print("The Si473X I2C address is 0x");
    Serial.println(si4735Addr, HEX);
  }
  si4735.setAutomaticGainControl(0, 26);

  delay(500);

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.D9 MHz; step 100kHz)
  si4735.setup(RESET_PIN, FM_FUNCTION); // sets to fm function
  si4735.setFM(6400, 10800, 10000, 10);
  delay(500);

  si4735.setVolume(63);
  // previous frequency and volume
  previousmenustate = get_menu();
  currentFrequency = previousFrequency = si4735.getFrequency();
  currentVolume = previousVolume = si4735.getVolume();

  si4735.setRdsConfig(3, 3, 3, 3, 3);
  si4735.setFifoCount(1);
  show_status();
  // ticker_rds.start();
  //----------------------------DISPLAY-------------------------------------------
  //  SETUP OF DISPLAY

  main_display();

  pinMode(frequency_up_button, INPUT_PULLDOWN);
  pinMode(frequency_down_button, INPUT_PULLDOWN);
  pinMode(volume_down_button, INPUT_PULLDOWN);
  pinMode(volume_up_button, INPUT_PULLDOWN);
  pinMode(toggle_menu_button, INPUT_PULLDOWN);
  LowPower.begin();

  // setup frequency up ISR
  LowPower.attachInterruptWakeup(frequency_up_button, frequency_up_pressed, RISING, SLEEP_MODE);
  // setup frequency down ISR
  LowPower.attachInterruptWakeup(frequency_down_button, frequency_down_pressed, RISING, SLEEP_MODE);
  // setup volume down ISR
  LowPower.attachInterruptWakeup(volume_down_button, volume_down_pressed, RISING, SLEEP_MODE);
  // setup volume up ISR
  LowPower.attachInterruptWakeup(volume_up_button, volume_up_pressed, RISING, SLEEP_MODE);
  // setup SWITCH MENU
  LowPower.attachInterruptWakeup(toggle_menu_button, toggle_menu, RISING, SLEEP_MODE);

  


}
void loop()
{
  // test of signal quality, audio, and SNR, print status.
  currentmenustate = get_menu();
  if (currentmenustate != previousmenustate)
  {

    if (menu_toggle)
    {
      // inladen menu 1
      main_display();
    }
    else
    {
      // inladen menu 2
      second_display();
    }
    previousmenustate = currentmenustate;
  }

  switch (i)
  {
  case -1:
    si4735.frequencyDown();
    i = 0;
    break;
  case 1:
    si4735.frequencyUp();
    i = 0;
    break;
  case -2:
    si4735.volumeDown();
    i = 0;
    break;
  case 2:
    si4735.volumeUp();
    i = 0;
    break;
  default:
    break;
  }

  currentFrequency = si4735.getCurrentFrequency();
  currentVolume = si4735.getCurrentVolume();
  // checken status, when frequecy changes

  if ((currentFrequency != previousFrequency))
  {
    previousFrequency = currentFrequency;
    frequency_display();
    if (menu_toggle)
    {
      SNR_display();
      signal_display();
      audio_display();
    }
    clearRdsBuffer();
    showProgramInfo();
    showRDSStation();
    show_status();
    // si4735.frequencyDown();
  }

  else if (currentVolume != previousVolume)
  {
    previousVolume = currentVolume;
    volume_display();
    SNR_display();

    signal_display();
    audio_display();
    show_status();
  }
  if (menu_toggle)
  {
    LowPower.sleep(); // hier wil ik dus gaan slapen en blijven slapen
  }
  else if (!menu_toggle)
  {
    checkRDS();
    LowPower.deepSleep(1000);
  }
}

