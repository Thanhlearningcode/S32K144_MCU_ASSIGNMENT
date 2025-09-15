#include "Application_BlinkLed.h"

void Delay_ms(unsigned int time)
{
    while (time--)
    {
        unsigned int i = 40000;
        while (i--);
    }
}

void Configure_LED_Pins(void)
{
    configure_ClockPort(PCC_PORTD_INDEX);
    configure_PortPin(PORTD, LED_RED_PIN, GPIO_OUTPUT);   // RED LED
    configure_PortPin(PORTD, LED_GREEN_PIN, GPIO_OUTPUT); // GREEN LED
    configure_PortPin(PORTD, LED_BLUE_PIN, GPIO_OUTPUT);  // BLUE LED
}


void TurnOff_All_LEDs(void)
{
    GPIO_SetPin(PORTD, LED_RED_PIN,     0);
    GPIO_SetPin(PORTD, LED_GREEN_PIN, 0);
    GPIO_SetPin(PORTD, LED_BLUE_PIN,    0);
}

void TurnOn_LED(uint8_t led_index)
{
    switch (led_index)
    {
        case 0:
            GPIO_SetPin(PORTD, LED_RED_PIN, 1);
            break;
        case 1:
            GPIO_SetPin(PORTD, LED_GREEN_PIN, 1);
            break;
        case 2:
            GPIO_SetPin(PORTD, LED_BLUE_PIN, 1);
            break;
        default:
            break;
    }
}
