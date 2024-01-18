#include <Arduino.h>
#include <SI4735.h>

extern SI4735 si4735;

extern uint16_t currentFrequency;
extern uint16_t previousFrequency;

extern uint8_t currentVolume;
extern uint8_t previousVolume;
extern volatile int i;
volatile int band_index = 0;
// functions

// function ISR 1

// function ISR 3
void volume_up_pressed()
{
    i = 2;
}

// function ISR 4
void volume_down_pressed()
{
    i = -2;
}

void show_status()
{
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
void frequency_up_pressed()
{

    i = 1;

}

// function ISR 2
void frequency_down_pressed()
{
    i = -1;
}
void band_up()
{
    // DEBUG
    if (band_index > 0)
    {
      //  second_display();
        band_index--;
    }
    si4735.setFmBandwidth(band_index);
}

void band_down()
{
    // second_display();
    if (band_index < 4)
    {
      //  second_display();
        band_index++;
    }
    si4735.setFmBandwidth(band_index);
}