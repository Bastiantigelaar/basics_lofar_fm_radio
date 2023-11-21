// header file for the TFT screen 

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
extern uint16_t currentFrequency;
extern uint16_t previousFrequency;

extern uint8_t currentVolume;
extern uint8_t previousVolume;

extern uint16_t currentSNR;
extern uint16_t currentRRSI;
extern String currentpilot;

Adafruit_ILI9341 tft = Adafruit_ILI9341(D7, D8, A6, A4, D10, A5);
// main/ default display
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
    tft.println(String(currentSNR) + "dB");

    tft.setCursor(10, 170);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Signal ");

    tft.setCursor(140, 170);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(currentRRSI) + "dBuV");

    tft.setCursor(10, 200);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("AUDIO ");

    tft.setCursor(140, 200);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(currentpilot));

    // tft.setCursor(70, 60);
    // tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    // tft.setTextSize(4);
    // tft.println(String(currentFrequency / 100.0, 2) + " MHz");
}
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
    tft.println(String(currentSNR) + "dB");
}
void signal_display()
{
    tft.setCursor(140, 170);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(currentRRSI) + "dBuV");
}
void audio_display()
{
    tft.setCursor(140, 200);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(currentpilot));
}
