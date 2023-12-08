//https://github.com/stm32duino/STM32LowPower/blob/main/examples/ExternalWakeup/ExternalWakeup.ino
// low power testen 
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
#include "STM32LowPower.h"

// fm chip declaration
SI4735 si4735;

// display [tft] declaration
Adafruit_ILI9341 tft = Adafruit_ILI9341(D7, D8, A6, A4, D10, A5);

//  functions prototypes
void frequency_up_pressed();
void frequency_down_pressed();
void volume_up_pressed();
void volume_down_pressed();

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

// controle function
void toggle_menu();

uint16_t currentFrequency;
uint16_t previousFrequency;
uint8_t currentVolume;
uint8_t previousVolume;

#define SSB 1 // delete

#define STORE_TIME 10000 // Time of inactivity to make the current receiver status writable (10s / 10000 milliseconds).

#define CLEAR_BUFFER(x) (x[0] = '\0');

const uint8_t app_id = 35; // Useful to check the EEPROM content before processing useful data
const int eeprom_address = 0;
long storeTime = millis();

bool bfoOn = false;
bool ssbLoaded = false;
bool fmStereo = true;

bool menu_toggle = true;

// buttons frequency up and down

int frequency_up_button = D12;
int frequency_down_button = D11;

// buttons volume up and down
int volume_up_button = D0;
int volume_down_button = D1;

// buttons band up and down

int band_up_button = 16;
int band_down_button = 17;

// button toggle stereo and mono // is this possible ??
// int toggle_stereo_mono = 29;

// RESET PIN FOR FM RADIO
#define RESET_PIN D9

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
  Serial.print("dit is de program info " + String(programInfo) + "\n");
  programInfo[60] = '\0'; // Removes unwanted characters from the RDS program information
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
  Serial.print("dit zijn de ingevulde parameters ... -> ");
  Serial.print("dit is de station naam" + String(stationName) + "\n");
  Serial.print("\n");
  si4735.removeUnwantedChar(stationName, 9);
  si4735.removeUnwantedChar(bufferStatioName, 9);
  delayStationName = millis();
}

void showRDSTime()
{
  if (rdsTime == NULL || (millis() - delatRdsTime) < 55000)
    return;
  Serial.print("dit is de rds tijd" + String(rdsTime) + "\n");
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
  while (!Serial)
    ;
  delay(20);

  Serial.print("IK STA AAN");
  // INDICATES THE PIN MODE
  pinMode(frequency_up_button, INPUT_PULLDOWN);
  pinMode(frequency_down_button, INPUT_PULLDOWN);
  pinMode(volume_down_button, INPUT_PULLDOWN);
  pinMode(volume_up_button, INPUT_PULLDOWN);

  // setup frequency up ISR

  // timer/ticker

  // reset pin
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);

  // check status FM chip
  // si4735.getStatus();
  // config_i2c_adress();
  delay(500);

  // Look for the Si47XX I2C bus address

  int16_t si4735Addr = si4735.getDeviceI2CAddress(D9);
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

  delay(500);

  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.D9 MHz; step 100kHz)
  si4735.setup(RESET_PIN, FM_FUNCTION); // sets to fm function
  si4735.setFM(6400, 10800, 10000, 10);
  delay(500);

  // TUNE FM CHIP TO FM

  // frequency values of the fm chip
  // note: every value needs to be multiplide with 10000
  // so for example 6400 means 64 Mhz, ect

  // default values
  // int //min_freq = 6400;
  // int //max_freq = 10800;
  // int default_freq = 9000;
  // int step_freq = 10;

  // setup FM functionality
  // si4735.setFM(min_freq, max_freq, default_freq, step_freq); // set the values that are defined above ^
  si4735.setVolume(20);
  // previous frequency and volume
  currentFrequency = previousFrequency = si4735.getFrequency();
  currentVolume = previousVolume = si4735.getVolume();

  si4735.setRdsConfig(3, 3, 3, 3, 3);
  si4735.setFifoCount(1);
  show_status();
  ticker_rds.start();
  //----------------------------DISPLAY-------------------------------------------
  // SETUP OF DISPLAY
  main_display();
// start of lower power mode ]
 
  LowPower.begin();
  LowPower.attachInterruptWakeup(digitalPinToInterrupt(frequency_up_button), frequency_up_pressed, RISING,SLEEP_MODE);
  // setup frequency down ISR
  LowPower.attachInterruptWakeup(digitalPinToInterrupt(frequency_down_button), frequency_down_pressed, RISING, SLEEP_MODE);
  // setup volume down ISR
  LowPower.attachInterruptWakeup(digitalPinToInterrupt(volume_down_button), toggle_menu, RISING, SLEEP_MODE);
  // setup volume up ISR
  LowPower.attachInterruptWakeup(digitalPinToInterrupt(volume_up_button), volume_up_pressed, RISING, SLEEP_MODE);
}
void loop()
{
  // test of signal quality, audio, and SNR, print status.
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
  if (!menu_toggle)
  {
    ticker_rds.update();
  }
  else if (menu_toggle)
  {
    ticker_rds.resume();
  }
 
  LowPower.sleep();
}
void frequency_up_pressed()
{

  // Serial.print("ik kom in de isr van de frequency up-button \n");
  si4735.frequencyUp(); // up with the step that was set
  //  Serial.print(" up mode - frequency " + String(currentFrequency / 100.0, 2));    // only for debug purposes

}
