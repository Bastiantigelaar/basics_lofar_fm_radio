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

extern char *programInfo;
extern char *stationName;
extern char *rdsTime;
extern char *stationInfo;

void update_station_naam();
void update_rds_time();
void update_program_info();
void battery();
void blokken(int aantalstrepen);
void delete_blokken(int aantalstrepen);
volatile int __previoscase = 0;
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

    // tft.begin();

    //  tft.fillScreen(ILI9341_WHITE);

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
}
void second_display()
{

    // tft.begin();

    // tft.fillScreen(ILI9341_WHITE);

    tft.setCursor(10, 110);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Station  ");

    tft.setCursor(140, 110);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(stationName));

    tft.setCursor(10, 140);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Tijd ");

    tft.setCursor(100, 140);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(rdsTime));

    tft.setCursor(10, 170);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Info ");

    tft.setCursor(140, 200);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(programInfo));
}
// update screen for ticker function
void update_menu()
{
    update_station_naam();
    update_rds_time();
    update_program_info();
}
// update station naam on display
void update_station_naam()
{
    if (stationName != NULL) // CHECK ALS DIE INGEVULD IS
    {
        // update screen
        tft.setCursor(140, 110);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.println(String(stationName));
    }
}
// update rds time on display
void update_rds_time()
{
    if (rdsTime != NULL) // CHECK ALS DIE IS INGEVULD
    {
        // update screen
        tft.setCursor(100, 140);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(3);
        tft.println(String(rdsTime));
    }
}
// update program info
void update_program_info()
{
    if (programInfo != NULL) // CHECK ALS DIE IS INGEVULD
    {
        // update screen
        tft.setCursor(10, 200);
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.setTextSize(2.9);
        tft.println(String(programInfo));
    }
}
void inverse_main_display()
{

    // tft.begin();

    //  tft.fillScreen(ILI9341_WHITE);
    // tft.setRotation(3.5);

    tft.setCursor(10, 110);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Volume ");

    tft.setCursor(140, 110);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(currentVolume));

    tft.setCursor(10, 140);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("SNR ");

    tft.setCursor(140, 140);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(si4735.getCurrentSNR()) + "dB");

    tft.setCursor(10, 170);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Signal ");

    tft.setCursor(140, 170);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(si4735.getCurrentRSSI()) + "dBuV");

    tft.setCursor(10, 200);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("AUDIO ");

    tft.setCursor(140, 200);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println((si4735.getCurrentPilot()) ? "STEREO" : "MONO");
}
void inverse_second_display()
{

    // tft.begin();

    // tft.fillScreen(ILI9341_WHITE);

    tft.setCursor(10, 110);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Station  ");

    tft.setCursor(140, 110);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(stationName));

    tft.setCursor(10, 140);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Tijd ");

    tft.setCursor(100, 140);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(rdsTime));

    tft.setCursor(10, 170);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("Info ");

    tft.setCursor(140, 200);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(String(programInfo));
}
void header_display()
{
    tft.begin();

    tft.fillScreen(ILI9341_WHITE);
    tft.setRotation(3.5);
    tft.setRotation(3.5);
    tft.setCursor(2, 5);
    tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft.setTextSize(3.5);
    tft.println("LOFAR-FM RADIO!");
    battery();
    tft.setCursor(70, 60);
    tft.setTextColor(ILI9341_RED, ILI9341_WHITE);
    tft.setTextSize(4.2);
    tft.println(String(currentFrequency / 100.0, 2) + " MHz");
}
void battery()
{
    // THIS WILL DRAW THE BATTERY ICON
    tft.setCursor(275, 5);
    tft.drawRect(275, 5, 40, 23, ILI9341_BLACK);
    // terminal of battey
    tft.setCursor(310, 5);
    tft.fillRect(315, 12, 3, 10, ILI9341_BLACK);
    blokken(4);
    delete_blokken(0);
}

void blokken(int aantalstrepen)
{

    int x = 278;
    int Xoffset = 0;

    for (int i = 0; i < aantalstrepen; i++)
    {
        int newx = x + Xoffset;
        tft.fillRect(newx, 8, 6, 18, ILI9341_GREEN);
        Xoffset += 9;
    }

    // tft.fillRect(277,8,8,18,ILI9341_GREEN);
}
void delete_blokken(int aantalstrepen)
{
    int x = 305; // begin waarde dus van rechts naar links!!!
                 // aka PVV naar GROENLINKS
    int Xoffset = 0;

    for (int i = 0; i < aantalstrepen; i++)
    {
        int newx = x + Xoffset;
        tft.fillRect(newx, 8, 6, 18, ILI9341_WHITE);
        Xoffset -= 9;
    }
}
void aanpassen(int level)
{
    if (__previoscase != level)
    {
        switch (level)
        {
        // 4 strepen
        case 0:
            // | | | | 
            blokken(4); 
            break;
        // 3 strepen
        case 1:
            // | | | 
            blokken(3);
            delete_blokken(1);
            break;
        // 2 strepen
        case 2:
            // | |
            blokken(2);
            delete_blokken(2);
            break;
        // 1 streep
        case 3:
            // | 
            blokken(1);
            delete_blokken(3);
            break;
        // 0 strepen
        case 4:
            // 
            blokken(0); 
            delete_blokken(4);
            break;
        default:
            break;
        }
        __previoscase = level;
    }
}
