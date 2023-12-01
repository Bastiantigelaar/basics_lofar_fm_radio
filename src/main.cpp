#include <Arduino.h>
// FM CHIP
#include <SI4735.h>
#include <control.h>
#include <FM_chip.h>

// DISPLAY
#include <TFT.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// fm chip declaration
SI4735 si4735;

// display [tft] declaration
 Adafruit_ILI9341 tft = Adafruit_ILI9341(D7, D8, A6, A4, D10, A5);
//Adafruit_ILI9341 tft = Adafruit_ILI9341(D7, D8,D10);
// functions prototypes
void frequency_up_pressed();
void frequency_down_pressed();
void volume_up_pressed();
void volume_down_pressed();
void config_i2c_adress();
void show_status();
void showStatus();
// void showHelp();

// DISPLAY FUNCTION
void main_display();
void volume_display();
void frequency_display();
void SNR_display();
void audio_display();
void signal_display();
uint16_t currentFrequency;
uint16_t previousFrequency;

uint8_t currentVolume;
uint8_t previousVolume;

// volatile float current = 100e6;

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

void setup()
{

  // defines and waits for serial com port
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(20);

  Serial.print("IK STA AAN");
  // INDICATES THE PIN MODE

  // setup frequency up ISR
  pinMode(frequency_up_button, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(frequency_up_button), frequency_up_pressed, RISING);

  // setup frequency down ISR
  pinMode(frequency_down_button, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(frequency_down_button), frequency_down_pressed, RISING);

  // setup volume down ISR
  pinMode(volume_down_button, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(volume_down_button), volume_down_pressed, RISING);

  // setup volume up ISR
  pinMode(volume_up_button, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(volume_up_button), volume_up_pressed, RISING);

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

  show_status();
  //----------------------------DISPLAY-------------------------------------------
  // SETUP OF DISPLAY
  main_display();
}
void loop()
{
  // test of signal quality, audio, and SNR, print status.
  currentFrequency = si4735.getCurrentFrequency();
  currentVolume = si4735.getCurrentVolume();
  // checken status, when frequecy changes
  if (currentFrequency != previousFrequency)
  {

    previousFrequency = currentFrequency;
    frequency_display();
    SNR_display();
    signal_display();
    audio_display();
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

  // si4735.frequencyUp();


  // LowPower.sleep();
}
void config_i2c_adress()
{
  // si4735.getStatus();
  int16_t i2cadress = si4735.getDeviceI2CAddress(RESET_PIN);
  if (i2cadress != 0)
  {
    Serial.print("devices has been found");
    Serial.print("the device has the follow i2c addres");
    Serial.print(i2cadress);
  }
  else
  {
    Serial.print("SI4735 not found, check connection, (else stament)");
  }
}