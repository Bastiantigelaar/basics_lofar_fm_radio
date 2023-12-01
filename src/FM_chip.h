#include <Arduino.h>
#include <SI4735.h>

extern SI4735 si4735;

extern uint16_t currentFrequency;
extern uint16_t previousFrequency;

extern uint8_t currentVolume;
extern uint8_t previousVolume;

extern char *utcTime;
extern char *stationName;
extern char *programInfo;
extern char *stationInfo;
void showProgramaInfo(); // you need check if programInfo is null in showProgramaInfo
void showStationName();  // you need check if stationName is null in showStationName
void showStationInfo();  // you need check if stationInfo is null in showStationInfo
void showUtcTime();
// functions

// function ISR 1
void frequency_up_pressed()
{

    Serial.print("ik kom in de isr van de frequency up-button \n");
    // si4735.frequencyUp(); // up with the step that was set
    //  Serial.print(" up mode - frequency " + String(currentFrequency / 100.0, 2));    // only for debug purposes
}

// function ISR 2
void frequency_down_pressed()
{
    Serial.print("ik kom in de isr van de frequency down-button \n");
    // si4735.frequencyDown(); // down with the step that was set
    //  Serial.print("down mode - frequency " + String(currentFrequency / 100.0, 2));    // only for debug puproses
}

// function ISR 3
void volume_up_pressed()
{
    si4735.volumeUp();                  // done with the step that was set
    Serial.print(" up mode - volume "); // only for debug puposes
}

// function ISR 4
void volume_down_pressed()
{
    si4735.volumeDown(); // down with the step that was set
                         // Serial.print("down mode - volume "); // only for debug purposes
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
void check_RDS_data()
{
    if (si4735.getRdsAllData(&stationName, &stationInfo, &programInfo, &utcTime))
    {
        showProgramaInfo(); // you need check if programInfo is null in showProgramaInfo
        showStationName();  // you need check if stationName is null in showStationName
        showStationInfo();  // you need check if stationInfo is null in showStationInfo
        showUtcTime();      // // you need check if rdsTime is null in showUtcTime
    }
    else
    {
        Serial.print("ik heb geen rds data kunnen vinden!!");
    }
}
void showStationName()
{
    if (stationName != NULL)
    {
        // update on screen
    }
}

void showStationInfo()
{
    if (stationInfo != NULL)
    {
        // update on screen
    }
}

void showProgramaInfo()
{
    if (programInfo != NULL)
    {
        // do update on screen
    }
}

void showUtcTime()
{
    if (utcTime != NULL)
    {
        // do update on screen
    }
}