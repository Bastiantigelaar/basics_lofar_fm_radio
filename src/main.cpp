#include <Arduino.h> // arduino library
#include <SI4735.h>
#include <control.h>
#include <FM_chip.h>
#include <TFT.h>

SI4735 si4735;

void frequency_up_pressed() // functie van isr 1
{
  si4735.frequencyUp();
  // current += 0.1e6;
  Serial.print("frequency up!"); // needs to be deleted after test
}
void frequency_down_pressed() // functie isr 2
{
  si4735.frequencyDown();
  // current -= 0.1e6;
  Serial.print("frequency down!"); // needs to be deleted after test
}

void frequency_up_pressed();
void frequency_down_pressed();

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


#define RESET_PIN PA12
#define FM_FUNCTION 1

void frequency_up_pressed() ;
void frequency_down_pressed() ;


void setup()
{
  Serial.begin(9600);
  delay(20); // wait for serial
  while (!Serial);
  si4735.getStatus();
  int16_t i2cadress = si4735.getDeviceI2CAddress(RESET_PIN);
  if (i2cadress != 0)
  {
    Serial.print("devices has been found");
  }
  else
  {
    Serial.print("SI4735 not found, check connection");
  }

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
