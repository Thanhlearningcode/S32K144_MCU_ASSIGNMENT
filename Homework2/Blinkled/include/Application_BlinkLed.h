#ifndef APPLICATION_BLINKLED_H_
#define APPLICATION_BLINKLED_H_

#include <Pcc.h>
#include "Port.h"

#define LED_RED_PIN    0    // RED LED trên PORTD, chân 0
#define LED_GREEN_PIN  15   // GREEN LED trên PORTD, chân 15
#define LED_BLUE_PIN   16   // BLUE LED trên PORTD, chân 16

void Delay_ms(unsigned int time);

void Configure_LED_Pins(void);

void TurnOff_All_LEDs(void);

void TurnOn_LED(uint8_t led_index);
#endif /* APPLICATION_BLINKLED_H_ */
