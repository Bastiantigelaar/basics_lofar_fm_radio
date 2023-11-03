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

uint16_t currentFrequency;

// volatile float current = 100e6;

// buttons pin configured
int frequency_up_button = 12;   // pin 12
int frequency_down_button = 11; // pin 11

int volume_down_button = 2;
int volume_up_button = 3;

// buttons pin configured
int band_up_button = 16;
int band_down_button = 17;
int toggle_stereo_mono = 29;

// #define RESET_PIN PA12
#define FM_FUNCTION 1
//#define RESET_PIN 9
void setup()
{
  Serial.begin(9600);
  // wait for serial com port
  while (!Serial)
    ;
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
   //pinMode(RESET_PIN, OUTPUT);

  // SET THE PINS
  // reset pin
   //digitalWrite(RESET_PIN,HIGH);

   //si4735.getStatus();
   //config_i2c_adress(); // chcks the i2c adress

  // delay(500); // to read the values easily

  // tune of FM CHIP
  // frequency values of the fm chip
  // note: every value needs to be multiplide with 10000
  // so for example 6400 means 64 Mhz, ect
  int min_freq = 6400;
  int max_freq = 10800;
  int default_freq = 9000;
  int step_freq = 10;

  //si4735.setup(RESET_PIN,FM_FUNCTION); // sets to fm function
  //si4735.setFM(min_freq, max_freq, default_freq, step_freq); // set the values that are defined above ^
  //si4735.setVolume(20);
}
void loop()
{

  // test for the frequency buttons..../

  Serial.print("this is the current frequency ");
  // Serial.print(si4735.getFrequency());
  Serial.print("\n");
  delay(1000);

  // test for the volume buttons ....

  Serial.print("this is the current volume level");
  //Serial.print(si4735.getVolume());
  Serial.print("\n");
  delay(1000);

  // LowPower.sleep();
}
// functions

void frequency_up_pressed() // functie van isr 1
{
  si4735.frequencyUp();                  // up with the step that was set
  Serial.print(" up mode - frequency "); // only for debug
}

void frequency_down_pressed() // functie isr 2
{
  si4735.frequencyDown();                 // down with the step that was set
  Serial.print("down mode - frequency "); // only for debug
}

void volume_up_pressed() // functie van isr 3
{
  // si4735.volumeUp();      // done with the step that was set
  Serial.print(" up mode - volume "); // only for debug
}
void volume_down_pressed() // functie isr 4
{
  si4735.volumeDown();                 // down with the step that was set
  Serial.print("down mode - volume "); // only for debug
}
/*
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
*/