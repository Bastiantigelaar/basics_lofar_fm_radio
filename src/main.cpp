#include <Arduino.h>
// FM CHIP
#include <SI4735.h>
#include <control.h>

// DISPLAY
#include <TFT.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// fm chip declaration
SI4735 si4735;

// display [tft] declaration
// Adafruit_ILI9341 tft = Adafruit_ILI9341(D7, D8, A6, A4, D10, A5);

// functions prototypes
void frequency_up_pressed();
void frequency_down_pressed();
void volume_up_pressed();
void volume_down_pressed();
void config_i2c_adress();
void show_status();
void showStatus();
void showHelp();

uint16_t currentFrequency;
uint16_t previousFrequency;

uint8_t currentVolume;
uint8_t previousVolume;

// volatile float current = 100e6;

// buttons frequency up and down

int frequency_up_button = D11;
int frequency_down_button = D10;

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

// FM function enabaled [0 == AM]
#define FM_FUNCTION 1

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
  si4735.setup(D9, FM_FUNCTION);
  // Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.D9 MHz; step 100kHz)
  si4735.setFM(8400, 10800, 10390, 10);
  delay(500);

  // TUNE FM CHIP TO FM

  // frequency values of the fm chip
  // note: every value needs to be multiplide with 10000
  // so for example 6400 means 64 Mhz, ect

  // default values
  int min_freq = 6400;
  int max_freq = 10800;
  int default_freq = 9000;
  int step_freq = 10;

  // setup FM functionality
  si4735.setup(RESET_PIN, FM_FUNCTION);                      // sets to fm function
  si4735.setFM(min_freq, max_freq, default_freq, step_freq); // set the values that are defined above ^
  si4735.setVolume(20);
  // previous frequency and volume
  currentFrequency = previousFrequency = si4735.getFrequency();
  currentVolume = previousVolume = si4735.getVolume();

  show_status();
  //----------------------------DISPLAY-------------------------------------------
  /* SETUP OF DISPLAY
  tft.begin();

  tft.se1tRotation(3);
  tft.setCursor(26, 5);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(6);
  tft.println("LOFAR-FM RADIO!");

  tft.setCursor(0, 100);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(3);
  tft.println("Bastian Tigelaar en Leon Weber");
  */
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
    show_status();
  }
  else if (currentVolume != previousVolume)
  {
    Serial.print("ik kom in de else if terecht");
    previousVolume = currentVolume;
    show_status();
  }

  // LowPower.sleep();
}
// functions

// function ISR 1
void frequency_up_pressed()
{
  si4735.frequencyUp(); // up with the step that was set
  // Serial.print(" up mode - frequency " + String(currentFrequency / 100.0, 2));    // only for debug purposes
}

// function ISR 2
void frequency_down_pressed()
{
  si4735.frequencyDown(); // down with the step that was set
  // Serial.print("down mode - frequency " + String(currentFrequency / 100.0, 2));    // only for debug puproses
}

// function ISR 3
void volume_up_pressed()
{
  si4735.volumeUp();                  // done with the step that was set
  Serial.print(" up mode - volume "); // only for debug puposes
}

// function ISR 4
void volume_down_pressed()
{
  si4735.volumeDown(); // down with the step that was set
  // Serial.print("down mode - volume "); // only for debug purposes
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

void show_status()
{
  si4735.getCurrentReceivedSignalQuality();
  Serial.print("You are tuned on ");
  if (si4735.isCurrentTuneFM())
  {
    Serial.print(String(currentFrequency / 100.0, 2));
    Serial.print("MHz ");
    Serial.print("\n");
    Serial.print("Dit is het volume" + String(si4735.getVolume()));

    Serial.print((si4735.getCurrentPilot()) ? "STEREO" : "MONO");
  }
  else
  {
    Serial.print(currentFrequency);
    Serial.print("kHz");
  }
  Serial.print(" [SNR:");
  Serial.print(si4735.getCurrentSNR());
  Serial.print("dB");

  Serial.print(" Signal:");
  Serial.print(si4735.getCurrentRSSI());
  Serial.println("dBuV]");
}

// -----------------------------------------DISPLAY ----------------------------------
/*void frequency_display()
{
  tft.setCursor(0, 160);
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println("De frequentie is " + String(currentFrequency));
}
void volume_display()
{
  tft.setCursor(0, 200);
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println("Het volume level is " + String());
}
*/
