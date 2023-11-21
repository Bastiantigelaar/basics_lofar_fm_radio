#include <Arduino.h> // arduino library
#include <SI4735.h>
#include <control.h>

#include <TFT.h>
// #include "ArduinoLowPower.h"
SI4735 si4735;

// functions prototypes
void frequency_up_pressed();
void frequency_down_pressed();
void volume_up_pressed();
void volume_down_pressed();
void config_i2c_adress();
void show_status();

uint16_t currentFrequency;

// volatile float current = 100e6;

// buttons frequency up and down

int frequency_up_button = 12;   
int frequency_down_button = 11; 

//buttons volume up and down
int volume_down_button = 2;
int volume_up_button = 3;

// buttons band up and down

int band_up_button = 16;
int band_down_button = 17;

// button toggle stereo and mono // is this possible ??
int toggle_stereo_mono = 29;

// RESET PIN FOR FM RADIO
#define RESET_PIN D9

// FM function enabaled [0 == AM]
#define FM_FUNCTION 1

void setup()
{

// defines and waits for serial com port
  Serial.begin(115200);
  while (!Serial);
  delay(20);

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
   digitalWrite(RESET_PIN,HIGH);

  // check status FM chip
    si4735.getStatus();
    config_i2c_adress(); 
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
    // TUNE FM CHIP TO FM

    // frequency values of the fm chip
    // note: every value needs to be multiplide with 10000
    // so for example 6400 means 64 Mhz, ect

    //default values
    int min_freq = 6400;
    int max_freq = 10800;
    int default_freq = 9000;
    int step_freq = 10;

    // setup FM functionality
    si4735.setup(RESET_PIN, FM_FUNCTION);                      // sets to fm function
    si4735.setFM(min_freq, max_freq, default_freq, step_freq); // set the values that are defined above ^
    si4735.setVolume(20);
    show_status();
}
void loop()
{
  // test of signal quality, audio, and SNR, print status. 
  show_status();
  delay(1000); 


  // LowPower.sleep();
}
// functions

// function ISR 1
void frequency_up_pressed() 
{
  si4735.frequencyUp();                  // up with the step that was set
  Serial.print(" up mode - frequency "); // only for debug purposes
}

// function ISR 2
void frequency_down_pressed() 
{
  si4735.frequencyDown();                 // down with the step that was set
  Serial.print("down mode - frequency "); // only for debug puproses
}

// function ISR 3 
void volume_up_pressed() 
{
  si4735.volumeUp();      // done with the step that was set
  Serial.print(" up mode - volume "); // only for debug puposes
}

// function ISR 4 
void volume_down_pressed() 
{
  si4735.volumeDown();                 // down with the step that was set
  Serial.print("down mode - volume "); // only for debug purposes
}


void config_i2c_adress(){
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


void show_status(){
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
