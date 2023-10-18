#include <Arduino.h> // arduino library 
#include <SI4735.h>
#include <control.h>
#include <FM_chip.h>
#include <TFT.h>



// buttons pin configured  
int frequency_up_button = 14;
int frequency_down_button = 15;
int volume_down_button = 11;
int volume_up_button = 12;
int band_up_button = 16;
int band_down_button = 17;
int toggle_stereo_mono = 29;

void frequency_up();

void setup() {
  Serial.begin(115200);
  while(!Serial);
  delay(20);
// FREQUENTIE UP ISR SETUP
  pinMode(frequency_up_button, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(frequency_up_button), frequency_up, CHANGE);
  // FREQUENCTIE DOWN ISR SETP 




}

void loop() {
  // put your main code here, to run repeatedly:


}
