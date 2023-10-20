#include <Arduino.h> // arduino library 
#include <SI4735.h>
#include <control.h>
#include <FM_chip.h>
#include <TFT.h>


volatile float current = 100e6;

// buttons pin configured
int frequency_up_button = 12;   // pin 12
int frequency_down_button = 11; // pin 11

// buttons pin configured
int frequency_up_button = 12;   // pin 12
int frequency_down_button = 11; // pin 11

int volume_down_button = 11;
int volume_up_button = 12;
int band_up_button = 16;
int band_down_button = 17;
int toggle_stereo_mono = 29;

void frequency_up_pressed() ;
void frequency_down_pressed() ;
SI4735 rx; 

void setup()
{
  Serial.begin(9600);

  // setup frequency up ISR
  pinMode(frequency_up_button, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(frequency_up_button), frequency_up_pressed, RISING);

  // setup frequency DOWN ISR
  pinMode(frequency_down_button, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(frequency_down_button), frequency_down_pressed, RISING);
}
void loop()
{
}
