#include <Arduino.h>
// DISPLAY
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// FM CHIP
#include <SI4735.h>


extern SI4735 si4735;
extern Adafruit_ILI9341 tft;

extern uint16_t currentFrequency;
extern uint16_t previousFrequency;

extern uint8_t currentVolume;
extern uint8_t previousVolume;

// -----------------------------------------DISPLAY ----------------------------------
void frequency_display()
{
    tft.setCursor(70, 60);
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.setTextSize(4.2);
    tft.println(String(currentFrequency / 100.0, 2));
}
void volume_display()
{
    tft.setCursor(140, 110);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(currentVolume));
}
void SNR_display()
{
    tft.setCursor(140, 140);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(si4735.getCurrentSNR()) + "dB");
}
void signal_display()
{
    tft.setCursor(140, 170);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(si4735.getCurrentRSSI()) + "dBuV");
}
void audio_display()
{
    tft.setCursor(140, 200);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println((si4735.getCurrentPilot()) ? "STEREO" : "MONO");
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
    tft.println(String(currentFrequency / 100.0, 2) + " MHz");

    tft.setCursor(10, 110);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Volume ");

    tft.setCursor(140, 110);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(currentVolume));

    tft.setCursor(10, 140);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("SNR ");

    tft.setCursor(140, 140);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(si4735.getCurrentSNR()) + "dB");

    tft.setCursor(10, 170);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Signal ");

    tft.setCursor(140, 170);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(si4735.getCurrentRSSI()) + "dBuV");

    tft.setCursor(10, 200);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("AUDIO ");

    tft.setCursor(140, 200);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println((si4735.getCurrentPilot()) ? "STEREO" : "MONO");

    // tft.setCursor(70, 60);
    // tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    // tft.setTextSize(4);
    // tft.println(String(currentFrequency / 100.0, 2) + " MHz");
}
void second_display()
{

    tft.begin();
    tft.
    tft.fillScreen(ILI9341_WHITE);
    tft.setRotation(3.5);
    tft.setCursor(26, 5);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3.5);
    tft.println("LOFAR-FM RADIO!");

    tft.setCursor(70, 60);
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.setTextSize(4.2);
    tft.println(String(currentFrequency / 100.0, 2) + " MHz");

    tft.setCursor(10, 110);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Station ");

    tft.setCursor(140, 110);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
  //  tft.println(String(currentVolume));

    tft.setCursor(10, 140);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Station info ");

    tft.setCursor(140, 140);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    //tft.println(String(si4735.getCurrentSNR()) + "dB");

    tft.setCursor(10, 170);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Programm info ");

    tft.setCursor(140, 170);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
   // tft.println(String(si4735.getCurrentRSSI()) + "dBuV");

    tft.setCursor(10, 200);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Tijd ");

    tft.setCursor(140, 200);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
   // tft.println((si4735.getCurrentPilot()) ? "STEREO" : "MONO");

    // tft.setCursor(70, 60);
    // tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    // tft.setTextSize(4);
    // tft.println(String(currentFrequency / 100.0, 2) + " MHz");
}
