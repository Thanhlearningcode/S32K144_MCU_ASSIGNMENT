/**
 * @file    port_driver.c
 * @brief   Minimal CMSIS-style PORT driver for S32K144
 * @version 0.1.0
 * @date    2025-09-20
 * @author  Nguyễn Văn Thành
 */
#include "port_driver.h"
#include <stddef.h>  /* for NULL */

void PORT_EnableClock(PORT_Type *portBase)
{
    if (portBase == PORTA)      { PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK; }
    else if (portBase == PORTB) { PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK; }
    else if (portBase == PORTC) { PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK; }
    else if (portBase == PORTD) { PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK; }
    else if (portBase == PORTE) { PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK; }
}

void PORT_PinMux(PORT_Type *portBase, uint32_t pin, uint32_t mux)
{
    /* Clear MUX then set MUX to requested ALT */
    portBase->PCR[pin] = (portBase->PCR[pin] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(mux);
}

void PORT_ConfigurePull(PORT_Type *portBase, uint32_t pin, const Port_PullConfigType *cfg)
{
    uint32_t pcr = portBase->PCR[pin];
    pcr &= ~(PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);

    if ((cfg != NULL) && cfg->enablePull) {
        pcr |= PORT_PCR_PE_MASK;
        if (cfg->pullUp) { pcr |= PORT_PCR_PS_MASK; }
    }
    portBase->PCR[pin] = pcr;
}
