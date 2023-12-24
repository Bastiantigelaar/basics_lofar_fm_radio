// this file includes the code for testing the fm chip and display simultaneously.
// this code is succesfully tested on the PCB v1.0 

// here the code:
/*
note:
Only this board is apperantly supported, use stm32 flash tool otherwise the code will not upload probably it is due some bug in platformio!


aanpassingen pcb:
- 3-5 extra leds -> status
- FM chip DBYP pin niet naar 3.3 volt maar na via c1 naar 3.3 volt
- display header omdraaien!!
-



test plan
- display -- done;
- FM chip -- done; 
- display and FM chip -- done; -- easy:))
- buttons display and FM chip
- RDS

now the FM chip will be tested

*/
#include <Arduino.h>

// DISPLAY
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <SI4735.h>

// the low power library.

// fm chip declaration

// display [tft] declaration
// Adafruit_ILI9341 tft = Adafruit_ILI9341(PB_12,PB_11,PB_10);

Adafruit_ILI9341 tft = Adafruit_ILI9341(PB_12, PB_11, PB_15, PB_13, PB_10, PB_14);

// fm chip declaration
SI4735 si4735;
int16_t si4735Addr;
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

// RESET PIN FOR FM RADIO
#define RESET_PIN PA_13

// FM function enabaled [1 == AM]
#define FM_FUNCTION 0

void setup()
{

  // defines and waits for serial com port
  Serial.begin(115200);
  delay(20);

  Serial.print("IK STA AAN");
  // INDICATES THE PIN MODE

  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  delay(500);

  si4735Addr = si4735.getDeviceI2CAddress(RESET_PIN);
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
  // si4735.setAutomaticGainControl(0, 26);

  // main_display();
  //  Starts defaul radio function and band (FM; from 84 to 108 MHz; 103.D9 MHz; step 100kHz)
  si4735.setup(RESET_PIN, FM_FUNCTION); // sets to fm function
  si4735.setFM(6400, 10800, 10000, 10);
  delay(500);
}
void loop()
{
  main_display();
  delay(1000);
}

// THIS IS THE DEFAULT SCREEN FOR THE 2.2 INCH DISPLAY
void main_display()
{

  tft.begin();

  tft.fillScreen(ILI9341_WHITE);
  tft.setRotation(3.5);
  tft.setCursor(26, 5);
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
  tft.setTextSize(3.5);
  tft.println("LOFAR-FM RADIO!");

  tft.setCursor(70, 60);
  tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
  tft.setTextSize(4.2);
  tft.println(String(si4735Addr));

  tft.setCursor(10, 110);
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println(String(si4735Addr, HEX));
}
