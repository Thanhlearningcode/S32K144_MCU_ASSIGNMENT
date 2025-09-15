#include "Application_BlinkLed.h"

int main(int argc, char* const argv[])
{
    Configure_LED_Pins();

    while (1)
    {
        for (uint8_t i = 0; i < 3; i++)  // Lặp qua các LED (Red -> Green -> Blue)
        {
            TurnOff_All_LEDs();
            TurnOn_LED(i);
            Delay_ms(3000);
        }
    }
    return 0;
}
