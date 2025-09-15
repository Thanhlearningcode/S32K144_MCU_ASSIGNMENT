#ifndef _PCC_LIBRARY_H_
#define _PCC_LIBRARY_H_


#include "System.h"

#define PCC_BASE                      0x40065000U    //< Page 625
#define PCC_PCCn_CGC_MASK   0x04000000U //<  Bitmask to enable clock for peripheral

#define PCC_PORTA_INDEX     0
#define PCC_PORTB_INDEX     1
#define PCC_PORTC_INDEX     2
#define PCC_PORTD_INDEX     22
#define PCC_GPIO_INDEX      18

typedef enum
{
    PCC_PORTA = PCC_PORTA_INDEX,
	PCC_PORTB = PCC_PORTB_INDEX,
	PCC_PORTC = PCC_PORTC_INDEX,
	PCC_PORTD = PCC_PORTD_INDEX,
	PCC_GPIO = PCC_GPIO_INDEX,
} PCC_PortIndex;

typedef struct {
    volatile uint32_t PR : 1;      // Present, bit 31
    volatile uint32_t CGC : 1;     // Clock Gate Control, bit 30
    volatile uint32_t RESERVED[30];
} PCC_Register;

#define PCC_PORTA           ((PCC_Register *)(PCC_BASE + 0x04 * PCC_PORTA_INDEX))  // PORTA
#define PCC_PORTB           ((PCC_Register *)(PCC_BASE + 0x04 * PCC_PORTB_INDEX))  // PORTB
#define PCC_PORTC           ((PCC_Register *)(PCC_BASE + 0x04 * PCC_PORTC_INDEX))  // PORTC
#define PCC_PORTD           ((PCC_Register *)(PCC_BASE + 0x04 * PCC_PORTD_INDEX))  // PORTD

static PCC_Register* const pcc_ports[] =
{
    (PCC_Register *)(PCC_BASE + 0x04 * PCC_PORTA_INDEX),  // PORTA
    (PCC_Register *)(PCC_BASE + 0x04 * PCC_PORTB_INDEX),  // PORTB
    (PCC_Register *)(PCC_BASE + 0x04 * PCC_PORTC_INDEX),  // PORTC
    (PCC_Register *)(PCC_BASE + 0x04 * PCC_PORTD_INDEX),  // PORTD
};



PCC_status configure_ClockPort (PCC_PortIndex  port_index);

#endif  /*_PCC_LIBRARY_H_*/
