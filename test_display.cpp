// this is an temp file just a sketch 
// display software test 
// performed by Bastian Tigelaar 
// update display every 1 second on PCB V1.0, tested succefully 
// code:::



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
- FM chip
- display and FM chip 
- buttons display and FM chip 
- RDS


*/
#include <Arduino.h>

// DISPLAY
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// the low power library.

// fm chip declaration


// display [tft] declaration
//Adafruit_ILI9341 tft = Adafruit_ILI9341(PB_12,PB_11,PB_10);

Adafruit_ILI9341 tft = Adafruit_ILI9341(PB_12, PB_11, PB_15,PB_13,PB_10,PB_14);


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


void setup()
{

  // defines and waits for serial com port
  Serial.begin(115200);
  delay(20);

  Serial.print("IK STA AAN");
  // INDICATES THE PIN MODE


  //main_display();
  
 
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
  tft.println(" MHz");

  tft.setCursor(10, 110);
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println("Volume ");

}
