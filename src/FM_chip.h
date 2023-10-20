#include <Arduino.h>
void frequency_up_pressed() // functie van isr 1
{
    current += 0.1e6;
    Serial.print(String(current) + "\n");
}
void frequency_down_pressed() // functie isr 2
{
    current -= 0.1e6;
    Serial.print(String(current) + "\n");
}