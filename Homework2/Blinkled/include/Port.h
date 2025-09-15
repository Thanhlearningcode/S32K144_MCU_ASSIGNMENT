#ifndef PORT_H_
#define PORT_H_

#include "System.h"

#define PORT_BASE         0x40049000U

#define PORTA_INDEX       0
#define PORTB_INDEX       1
#define PORTC_INDEX       2
#define PORTD_INDEX       3

typedef struct
{
    volatile uint32_t PCR[32];  // Pin Control Register (32 pins)
    volatile uint32_t GPCLR;    // Global Pin Control Low Register
    volatile uint32_t GPCHR;    // Global Pin Control High Register
} PORT_Type;

#define PORTA        ((PORT_Type *)(PORT_BASE + 0x40 * PORTA_INDEX))
#define PORTB        ((PORT_Type *)(PORT_BASE + 0x40 * PORTB_INDEX))
#define PORTC        ((PORT_Type *)(PORT_BASE + 0x40 * PORTC_INDEX))
#define PORTD        ((PORT_Type *)(PORT_BASE + 0x40 * PORTD_INDEX))


typedef enum
{
    GPIO_OUTPUT = 0,
    GPIO_INPUT = 1,
    GPIO_INPUT_PULLUP = 2,
    GPIO_INPUT_PULLDOWN = 3,
} GPIO_Mode;

Port_status configure_PortPin (PORT_Type *port, uint8_t  pin, GPIO_Mode mode);

void GPIO_SetPin(PORT_Type *port, uint8_t  pin, uint8_t state);
#endif /* PORT_H_ */
